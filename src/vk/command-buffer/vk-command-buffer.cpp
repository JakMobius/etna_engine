
#include <etna/vk-wrappers/command-buffer/vk-command-buffer.hpp>
#include <etna/vk-wrappers/command-buffer/vk-command-pool.hpp>
#include <etna/vk-wrappers/queue/vk-queue.hpp>
#include <etna/vk-wrappers/render-pass/vk-render-pass-begin-info.hpp>

void VK::CommandBufferBase::reset(VkCommandBufferResetFlags flags) {
    vkResetCommandBuffer(m_handle, flags);
}

void VK::CommandBufferBase::begin(VkCommandBufferUsageFlags flags) {
    VkCommandBufferBeginInfo begin_info {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = flags;

    if (vkBeginCommandBuffer(m_handle, &begin_info) != VK_SUCCESS) {
        throw std::runtime_error("failed to start recording command buffer");
    }
}

void VK::CommandBufferBase::end() {
    if (vkEndCommandBuffer(m_handle) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }
}

void VK::CommandBufferBase::submit_and_wait(const VK::Queue& queue, const VK::UnownedFence& fence,
                                            std::span<VkSemaphore> signal_semaphores,
                                            std::span<VkSemaphore> wait_semaphores,
                                            std::span<VkPipelineStageFlags> wait_stages) {
    submit(queue, fence, signal_semaphores, wait_semaphores, wait_stages);
    queue.wait_idle();
}


void VK::CommandBufferBase::submit(const VK::Queue& queue, const VK::UnownedFence& fence,
                                   std::span<VkSemaphore> signal_semaphores,
                                   std::span<VkSemaphore> wait_semaphores,
                                   std::span<VkPipelineStageFlags> wait_stages) {
    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submit_info.signalSemaphoreCount = signal_semaphores.size();
    submit_info.pSignalSemaphores = signal_semaphores.data();
    submit_info.waitSemaphoreCount = wait_semaphores.size();
    submit_info.pWaitSemaphores = wait_semaphores.data();
    submit_info.pWaitDstStageMask = wait_stages.data();

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_handle;

    if(vkQueueSubmit(queue.get_handle(), 1, &submit_info, fence.get_handle()) != VK_SUCCESS){
        throw std::runtime_error("failed to submit command buffer");
    }
}

void
VK::CommandBufferBase::bind_vertex_buffers(std::span<const VkBuffer> buffers, std::span<const VkDeviceSize> offsets) {
    vkCmdBindVertexBuffers(m_handle, 0, buffers.size(), buffers.data(), offsets.data());
}

void VK::CommandBufferBase::bind_vertex_buffers(uint32_t first_binding, uint32_t binding_count, const VkBuffer* buffers,
                                                const VkDeviceSize* offsets) {
    vkCmdBindVertexBuffers(m_handle, first_binding, binding_count, buffers, offsets);
}

void VK::CommandBufferBase::bind_index_buffer(const VK::Buffer &buffer, VkDeviceSize offset, VkIndexType index_type) {
    vkCmdBindIndexBuffer(m_handle, buffer.get_handle(), offset, index_type);
}

void VK::CommandBufferBase::bind_index_buffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType index_type) {
    vkCmdBindIndexBuffer(m_handle, buffer, offset, index_type);
}

void
VK::CommandBufferBase::bind_descriptor_sets(VkPipelineBindPoint bind_point, VkPipelineLayout layout, uint32_t first_set,
                                            std::span<VkDescriptorSet> descriptor_sets,
                                            std::span<uint32_t> dynamic_offsets) {
    vkCmdBindDescriptorSets(m_handle, bind_point, layout, first_set, descriptor_sets.size(), descriptor_sets.data(), dynamic_offsets.size(), dynamic_offsets.data());
}

void VK::CommandBufferBase::bind_descriptor_sets(VkPipelineBindPoint bind_point, VkPipelineLayout layout,
                                                 std::span<VkDescriptorSet> descriptor_sets,
                                                 std::span<uint32_t> dynamic_offsets) {
    vkCmdBindDescriptorSets(m_handle, bind_point, layout, 0, descriptor_sets.size(), descriptor_sets.data(), dynamic_offsets.size(), dynamic_offsets.data());
}

void VK::CommandBufferBase::draw_indexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index,
                                         int32_t vertex_offset, uint32_t first_instance) {
    vkCmdDrawIndexed(m_handle, index_count, instance_count, first_index, vertex_offset, first_instance);
}

void VK::CommandBufferBase::draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) {
    vkCmdDraw(m_handle, vertex_count, instance_count, first_vertex, first_instance);
}

void VK::CommandBufferBase::bind_pipeline(VkPipelineBindPoint bind_point, const VK::Pipeline &pipeline) {
    vkCmdBindPipeline(m_handle, bind_point, pipeline.get_handle());
}

void VK::CommandBufferBase::bind_pipeline(VkPipelineBindPoint bind_point, VkPipeline pipeline) {
    vkCmdBindPipeline(m_handle, bind_point, pipeline);
}

void VK::CommandBufferBase::begin_render_pass(const RenderPassBeginInfo& begin, VkSubpassContents contents) {
    begin_render_pass(&(begin.get_description()), contents);
}

void VK::CommandBufferBase::begin_render_pass(const VkRenderPassBeginInfo* begin, VkSubpassContents contents) {
    vkCmdBeginRenderPass(m_handle, begin, contents);
}

void VK::CommandBufferBase::end_render_pass() {
    vkCmdEndRenderPass(m_handle);
}

void VK::CommandBuffer::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkFreeCommandBuffers(this->m_device->get_handle(), this->m_command_pool->get_handle(), 1, &this->m_handle);
    this->m_handle = nullptr;
}
