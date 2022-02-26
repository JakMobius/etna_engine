#pragma once

namespace VK {

class CommandBuffer;

}

#include <span>
#include "vk-device.hpp"
#include "vk-command-pool.hpp"
#include "pipeline/vk-pipeline.hpp"
#include "buffer/vk-buffer.hpp"

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

    void reset(VkCommandBufferResetFlags flags = 0) {
        vkResetCommandBuffer(m_handle, flags);
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

    void bind_vertex_buffers(std::span<const VkBuffer> buffers, std::span<const VkDeviceSize> offsets) {
        vkCmdBindVertexBuffers(m_handle, 0, buffers.size(), buffers.data(), offsets.data());
    }

    void bind_vertex_buffers(uint32_t first_binding, uint32_t binding_count, const VkBuffer* buffers, const VkDeviceSize* offsets) {
        vkCmdBindVertexBuffers(m_handle, first_binding, binding_count, buffers, offsets);
    }

    void bind_index_buffer(const Buffer& buffer, VkDeviceSize offset, VkIndexType index_type) {
        vkCmdBindIndexBuffer(m_handle, buffer.get_handle(), offset, index_type);
    }

    void bind_index_buffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType index_type) {
        vkCmdBindIndexBuffer(m_handle, buffer, offset, index_type);
    }

    void bind_descriptor_sets(VkPipelineBindPoint bind_point, VkPipelineLayout layout, uint32_t first_set, std::span<VkDescriptorSet> descriptor_sets, std::span<uint32_t> dynamic_offsets) {
        vkCmdBindDescriptorSets(m_handle, bind_point, layout, first_set, descriptor_sets.size(), descriptor_sets.data(), dynamic_offsets.size(), dynamic_offsets.data());
    }

    void bind_descriptor_sets(VkPipelineBindPoint bind_point, VkPipelineLayout layout, std::span<VkDescriptorSet> descriptor_sets, std::span<uint32_t> dynamic_offsets) {
        vkCmdBindDescriptorSets(m_handle, bind_point, layout, 0, descriptor_sets.size(), descriptor_sets.data(), dynamic_offsets.size(), dynamic_offsets.data());
    }

    void draw_indexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance) {
        vkCmdDrawIndexed(m_handle, index_count, instance_count, first_index, vertex_offset, first_instance);
    }

    void bind_pipeline(VkPipelineBindPoint bind_point, const Pipeline& pipeline) {
        vkCmdBindPipeline(m_handle, bind_point, pipeline.get_handle());
    }

    void bind_pipeline(VkPipelineBindPoint bind_point, VkPipeline pipeline) {
        vkCmdBindPipeline(m_handle, bind_point, pipeline);
    }

    void begin_render_pass(const VkRenderPassBeginInfo* begin, VkSubpassContents contents) {
        vkCmdBeginRenderPass(m_handle, begin, contents);
    }

    void end_render_pass() {
        vkCmdEndRenderPass(m_handle);
    }

    std::vector<VkSemaphore>& get_signal_semaphores() { return m_signal_semaphores; }
    std::vector<VkSemaphore>& get_wait_semaphores() { return m_wait_semaphores; }
    std::vector<VkPipelineStageFlags>& get_wait_flags() { return wait_stage_masks; }

    VkCommandBuffer get_handle() { return m_handle; }

    ~CommandBuffer();
};

}