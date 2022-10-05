
#include <etna/vk-wrappers/barriers/vk-buffer-memory-barrier.hpp>

VK::BufferMemoryBarrier::BufferMemoryBarrier(const VK::Buffer& buffer) {
    m_description.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;

    m_description.buffer = buffer.get_handle();
    m_description.size = VK_WHOLE_SIZE;

    m_description.srcAccessMask = VK_ACCESS_FLAG_BITS_MAX_ENUM;
    m_description.dstAccessMask = VK_ACCESS_FLAG_BITS_MAX_ENUM;
}

void VK::BufferMemoryBarrier::write(const UnownedCommandBuffer& command_buffer, VkPipelineStageFlags src_mask,
                                   VkPipelineStageFlags dst_mask) {
    vkCmdPipelineBarrier(command_buffer.get_handle(),
                         src_mask, dst_mask, 0,
                         0, nullptr,
                         1, &m_description,
                         0, nullptr);
}
