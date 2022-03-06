#pragma once

#include "../image/vk-image.hpp"
#include "../image/vk-image-subresource-range.hpp"
#include "../command-buffer.hpp"

namespace VK {

class ImageMemoryBarrier {

    VkImageMemoryBarrier m_description {};

public:
    explicit ImageMemoryBarrier(const UnownedImage& image);

    ImageSubresourceRange get_subresource_range() { return ImageSubresourceRange::existing(m_description.subresourceRange); }

    void set_layouts(VkImageLayout src_layout, VkImageLayout dst_layout) {
        m_description.oldLayout = src_layout;
        m_description.newLayout = dst_layout;
    }

    void set_access_masks(VkAccessFlags src_masks, VkAccessFlags dst_masks) {
        m_description.srcAccessMask = src_masks;
        m_description.dstAccessMask = dst_masks;
    }

    VkImageMemoryBarrier& get_description() {
        return m_description;
    }

    void write(CommandBuffer* command_buffer, VkPipelineStageFlags src_mask, VkPipelineStageFlags dst_mask);
};

}
