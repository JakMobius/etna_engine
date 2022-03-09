#pragma once

#include <unordered_map>
#include <etna/image.hpp>
#include <etna/vk-wrappers/queue/vk-queue.hpp>
#include <etna/vk-wrappers/command-buffer/vk-command-buffer.hpp>

namespace Etna {

class CommandImage;
class CommandQueue {

    std::span<VkPipelineStageFlags> m_wait_stages;
    std::span<VkSemaphore> m_wait_semaphores;
    std::span<VkSemaphore> m_signal_semaphores;
    VK::CommandBuffer m_command_buffer;
    VK::Queue m_queue;
    bool m_is_ended = false;

public:
    CommandQueue(VK::CommandBuffer&& buffer, const VK::Queue& queue): m_command_buffer(std::move(buffer)), m_queue(queue) {}
    CommandQueue(CommandQueue&& move) noexcept = default;
    CommandQueue& operator=(CommandQueue&& move_assign) noexcept = default;

    CommandImage provide_image_state(Image* image, const ImmediateImageState& state);

    VK::CommandBuffer& get_command_buffer() { return m_command_buffer; }
    const VK::CommandBuffer& get_command_buffer() const { return m_command_buffer; }

    CommandQueue& set_wait_semaphores(std::span<VkSemaphore> semaphores) {
        m_wait_semaphores = semaphores;
        return *this;
    }

    CommandQueue& set_signal_semaphores(std::span<VkSemaphore> semaphores) {
        m_signal_semaphores = semaphores;
        return *this;
    }

    CommandQueue& set_wait_stages(std::span<VkPipelineStageFlags> wait_stages) {
        m_wait_stages = wait_stages;
        return *this;
    }

    CommandQueue& reset() {
        m_is_ended = false;
        m_command_buffer.reset();
        return *this;
    }

    CommandQueue& begin(VkCommandBufferUsageFlags flags = 0) {
        m_command_buffer.begin(flags);
        return *this;
    }

    CommandQueue& end() {
        m_is_ended = true;
        m_command_buffer.end();
        return *this;
    }

    CommandQueue& run(const VK::Fence& fence = {}) {
        m_command_buffer.submit(m_queue, fence, m_signal_semaphores, m_wait_semaphores, m_wait_stages);
        return *this;
    }

    CommandQueue& run_sync(const VK::Fence& fence = {}) {
        run(fence);
        m_queue.wait_idle();
        return *this;
    }

};

}