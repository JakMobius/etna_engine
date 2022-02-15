#pragma once

namespace VK {

class CommandBuffer;

}

#include "vk-device.hpp"
#include "vk-command-pool.hpp"

namespace VK {

class CommandBuffer {

    VkCommandBuffer m_handle;
    VK::CommandPool* m_pool;
    std::vector<VkSemaphore> m_signal_semaphores {};
    std::vector<VkSemaphore> m_wait_semaphores {};
    std::vector<VkPipelineStageFlags> wait_stage_masks {};

public:
    CommandBuffer(VkCommandBuffer handle, VK::CommandPool* pool): m_handle(handle), m_pool(pool) {

    }

    CommandBuffer(CommandBuffer&& move) {
        m_handle = move.m_handle;
        m_pool = move.m_pool;
        move.m_handle = nullptr;
    }

    void begin(VkCommandBufferUsageFlags flags) {
        VkCommandBufferBeginInfo begin_info {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = flags;

        if (vkBeginCommandBuffer(m_handle, &begin_info) != VK_SUCCESS) {
            throw std::runtime_error("failed to start recording command buffer");
        }
    }

    void end() {
        if (vkEndCommandBuffer(m_handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer");
        }
    }

    void submit_and_wait(VkQueue queue, VkFence fence) {
        submit(queue, fence);
        vkQueueWaitIdle(queue);
    }

    void submit(VkQueue queue, VkFence fence) {
        VkSubmitInfo submit_info {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        submit_info.signalSemaphoreCount = m_signal_semaphores.size();
        submit_info.pSignalSemaphores = m_signal_semaphores.data();

        submit_info.waitSemaphoreCount = m_wait_semaphores.size();
        submit_info.pWaitSemaphores = m_wait_semaphores.data();

        submit_info.pWaitDstStageMask = wait_stage_masks.data();

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &m_handle;

        if(vkQueueSubmit(queue, 1, &submit_info, fence) != VK_SUCCESS){
            throw std::runtime_error("failed to submit command buffer");
        }
    }

    std::vector<VkSemaphore>& get_signal_semaphores() { return m_signal_semaphores; }
    std::vector<VkSemaphore>& get_wait_semaphores() { return m_wait_semaphores; }
    std::vector<VkPipelineStageFlags>& get_wait_flags() { return wait_stage_masks; }

    VkCommandBuffer get_handle() { return m_handle; }

    ~CommandBuffer();
};

}