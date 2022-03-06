#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class ImageSubresourceRange {
    VkImageSubresourceRange m_description {};
public:
    ImageSubresourceRange() {
        m_description.baseArrayLayer = 0;
        m_description.levelCount = 1;
        m_description.baseMipLevel = 0;
        m_description.levelCount = 1;
        m_description.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }
    ImageSubresourceRange(const ImageSubresourceRange& copy) = default;
    ImageSubresourceRange(ImageSubresourceRange&& move) noexcept = default;
    ImageSubresourceRange& operator=(const ImageSubresourceRange& copy_assign) = default;
    ImageSubresourceRange& operator=(ImageSubresourceRange&& move_assign) noexcept = default;

    static ImageSubresourceRange& existing(VkImageSubresourceRange& range) {
        // Not certain is it legal, actually...
        return *((ImageSubresourceRange*)((char*)&range - offsetof(ImageSubresourceRange, m_description)));
    }

    static const ImageSubresourceRange& existing(const VkImageSubresourceRange& range) {
        // Not certain is it legal, actually...
        return *((const ImageSubresourceRange*)((char*)&range - offsetof(ImageSubresourceRange, m_description)));
    }

    uint32_t get_mip_levels()            const { return m_description.levelCount; }
    uint32_t get_base_mip_level()        const { return m_description.baseMipLevel; }
    uint32_t get_array_layers()          const { return m_description.layerCount; }
    uint32_t get_base_array_layer()      const { return m_description.baseArrayLayer; }
    VkImageAspectFlags get_aspect_mask() const { return m_description.aspectMask; }

    ImageSubresourceRange& set_mip_levels(uint32_t mip_levels) {
        m_description.levelCount = mip_levels;
        return *this;
    }

    ImageSubresourceRange& set_base_mip_level(uint32_t base_mip_level) {
        m_description.baseMipLevel = base_mip_level;
        return *this;
    }

    ImageSubresourceRange& set_array_layers(uint32_t array_layers) {
        m_description.layerCount = array_layers;
        return *this;
    }

    ImageSubresourceRange& set_base_array_layer(uint32_t base_array_layer) {
        m_description.baseArrayLayer = base_array_layer;
        return *this;
    }

    ImageSubresourceRange& set_aspect_mask(VkImageAspectFlags mask) {
        m_description.aspectMask = mask;
        return *this;
    }

    static ImageSubresourceRange single_mip_level(uint32_t level) {
        ImageSubresourceRange result {};
        result.m_description.baseMipLevel = level;
        return result;
    }

    static ImageSubresourceRange mip_levels(uint32_t count) {
        ImageSubresourceRange result {};
        result.m_description.levelCount = count;
        return result;
    }

    static ImageSubresourceRange mip_levels(uint32_t base, uint32_t count) {
        ImageSubresourceRange result {};
        result.m_description.baseMipLevel = base;
        result.m_description.levelCount = count;
        return result;
    }

    const VkImageSubresourceRange& get_description() const { return m_description; }
    VkImageSubresourceRange& get_description() { return m_description; }
};

}