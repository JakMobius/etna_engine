//
// Created by Артем on 13.02.2022.
//

#include "vk-command-pool.hpp"

VK::CommandBuffer VK::CommandPool::create_command_buffer() {
    VkCommandBufferAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = m_handle;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;

    // TODO: this method can only create a single command buffer.
    // Vulkan can create several command buffers at once, which may
    // be more efficient.

    VkCommandBuffer command_buffer = nullptr;

    if (vkAllocateCommandBuffers(m_surface_context->get_device()->get_handle(), &alloc_info, &command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }

    return VK::CommandBuffer(command_buffer, this);
}

VK::CommandPool::~CommandPool() {
    vkDestroyCommandPool(m_surface_context->get_device()->get_handle(), m_handle, nullptr);
}
