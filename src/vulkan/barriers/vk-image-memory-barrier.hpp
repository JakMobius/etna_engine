#pragma once

#include "../image/vk-image.hpp"
#include "../command-buffer.hpp"

namespace VK {

class ImageMemoryBarrier {

    VkImageMemoryBarrier m_description {};

public:
    explicit ImageMemoryBarrier(Image* image);

    void set_aspect_mask(VkImageAspectFlags flags) {
        m_description.subresourceRange.aspectMask = flags;
    }

    void set_mip_level_base(uint32_t level_base) {
        m_description.subresourceRange.baseMipLevel = level_base;
    }

    void set_mip_level_count(uint32_t level_count) {
        m_description.subresourceRange.levelCount = level_count;
    }

    void set_layer_base(uint32_t layer_base) {
        m_description.subresourceRange.layerCount = layer_base;
    }

    void set_layer_count(uint32_t layer_count) {
        m_description.subresourceRange.layerCount = layer_count;
    }

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
