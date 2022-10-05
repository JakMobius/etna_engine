
#include <etna/vk-wrappers/barriers/vk-memory-barrier.hpp>

VK::MemoryBarrier::MemoryBarrier() {
    m_description.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;

    m_description.srcAccessMask = VK_ACCESS_FLAG_BITS_MAX_ENUM;
    m_description.dstAccessMask = VK_ACCESS_FLAG_BITS_MAX_ENUM;
}

void VK::MemoryBarrier::write(const UnownedCommandBuffer& command_buffer, VkPipelineStageFlags src_mask,
                                   VkPipelineStageFlags dst_mask) {
    vkCmdPipelineBarrier(command_buffer.get_handle(),
                         src_mask, dst_mask, 0,
                         1, &m_description,
                         0, nullptr,
                         0, nullptr);
}
