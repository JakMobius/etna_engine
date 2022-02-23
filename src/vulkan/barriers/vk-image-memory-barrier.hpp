#pragma once

#include "../vk-image-2d.hpp"

namespace VK {

class ImageMemoryBarrier {

    VkImageMemoryBarrier m_description {};

public:
    explicit ImageMemoryBarrier(Image2D* image) {
        m_description.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        m_description.image = image->get_handle();

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
};

}