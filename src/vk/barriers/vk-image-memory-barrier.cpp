
#include <etna/vk-wrappers/barriers/vk-image-memory-barrier.hpp>

VK::ImageMemoryBarrier::ImageMemoryBarrier(const VK::UnownedImage& image) {
    m_description.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    m_description.image = image.get_handle();

    m_description.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    m_description.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    m_description.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_description.subresourceRange.baseArrayLayer = 0;
    m_description.subresourceRange.layerCount = 1;
    m_description.subresourceRange.levelCount = 1;
    m_description.subresourceRange.baseMipLevel = 0;
    m_description.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    m_description.newLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    m_description.srcAccessMask = VK_ACCESS_FLAG_BITS_MAX_ENUM;
    m_description.dstAccessMask = VK_ACCESS_FLAG_BITS_MAX_ENUM;
}

void VK::ImageMemoryBarrier::write(const UnownedCommandBuffer& command_buffer, VkPipelineStageFlags src_mask,
                                   VkPipelineStageFlags dst_mask) {
    vkCmdPipelineBarrier(command_buffer.get_handle(),
                         src_mask, dst_mask, 0,
                         0, nullptr,
                         0, nullptr,
                         1, &m_description);
}
