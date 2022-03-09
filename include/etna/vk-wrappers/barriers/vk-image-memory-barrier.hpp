#pragma once

#include <etna/vk-wrappers/image/vk-image.hpp>
#include <etna/vk-wrappers/image/vk-image-subresource-range.hpp>
#include <etna/vk-wrappers/command-buffer.hpp>

namespace VK {

class ImageMemoryBarrier {

    VkImageMemoryBarrier m_description {};

public:
    explicit ImageMemoryBarrier(const UnownedImage& image);

    ImageSubresourceRange& get_subresource_range() { return ImageSubresourceRange::existing(m_description.subresourceRange); }
    const ImageSubresourceRange& get_subresource_range() const { return ImageSubresourceRange::existing(m_description.subresourceRange); }

    ImageMemoryBarrier& set_layouts(VkImageLayout src_layout, VkImageLayout dst_layout) {
        m_description.oldLayout = src_layout;
        m_description.newLayout = dst_layout;
        return *this;
    }

    ImageMemoryBarrier& set_access_masks(VkAccessFlags src_masks, VkAccessFlags dst_masks) {
        m_description.srcAccessMask = src_masks;
        m_description.dstAccessMask = dst_masks;
        return *this;
    }

    VkImageMemoryBarrier& get_description() {
        return m_description;
    }

    void write(const UnownedCommandBuffer& command_buffer, VkPipelineStageFlags src_mask, VkPipelineStageFlags dst_mask);
};

}
