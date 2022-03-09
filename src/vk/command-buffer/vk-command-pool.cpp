
#include <etna/vk-wrappers/command-buffer/vk-command-pool.hpp>
#include <etna/vk-wrappers/command-buffer/vk-command-buffer.hpp>

VK::CommandBuffer VK::CommandPoolBase::create_command_buffer(VkCommandBufferLevel level) {
    VkCommandBufferAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = m_handle;
    alloc_info.level = level;
    alloc_info.commandBufferCount = 1;

    // TODO: this method can only create a single command buffer.
    // Vulkan can create several command buffers at once, which may
    // be more efficient.

    VkCommandBuffer command_buffer = nullptr;

    if (vkAllocateCommandBuffers(m_device->get_handle(), &alloc_info, &command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }

    return CommandBuffer { m_device, this, command_buffer };
}
