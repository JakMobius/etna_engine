#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class ImageSubresourceLayers {
    VkImageSubresourceLayers m_description {};
public:
    ImageSubresourceLayers() {
        m_description.baseArrayLayer = 0;
        m_description.layerCount = 1;
        m_description.mipLevel = 0;
        m_description.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }
    ImageSubresourceLayers(const ImageSubresourceLayers& copy) = default;
    ImageSubresourceLayers(ImageSubresourceLayers&& move) noexcept = default;
    ImageSubresourceLayers& operator=(const ImageSubresourceLayers& copy_assign) = default;
    ImageSubresourceLayers& operator=(ImageSubresourceLayers&& move_assign) noexcept = default;

    static ImageSubresourceLayers& existing(VkImageSubresourceLayers& range) {
        // Not certain is it legal, actually...
        return *((ImageSubresourceLayers*)((char*)&range - offsetof(ImageSubresourceLayers, m_description)));
    }

    static const ImageSubresourceLayers& existing(const VkImageSubresourceLayers& range) {
        // Not certain is it legal, actually...
        return *((const ImageSubresourceLayers*)((char*)&range - offsetof(ImageSubresourceLayers, m_description)));
    }

    uint32_t get_mip_level()            const { return m_description.mipLevel; }
    uint32_t get_array_layers()          const { return m_description.layerCount; }
    uint32_t get_base_array_layer()      const { return m_description.baseArrayLayer; }
    VkImageAspectFlags get_aspect_mask() const { return m_description.aspectMask; }

    ImageSubresourceLayers& set_mip_level(uint32_t base_mip_level) {
        m_description.mipLevel = base_mip_level;
        return *this;
    }

    ImageSubresourceLayers& set_array_layers(uint32_t array_layers) {
        m_description.layerCount = array_layers;
        return *this;
    }

    ImageSubresourceLayers& set_base_array_layer(uint32_t base_array_layer) {
        m_description.baseArrayLayer = base_array_layer;
        return *this;
    }

    ImageSubresourceLayers& set_aspect_mask(VkImageAspectFlags mask) {
        m_description.aspectMask = mask;
        return *this;
    }

    static ImageSubresourceLayers single_layer(uint32_t layer) {
        ImageSubresourceLayers result {};
        result.m_description.baseArrayLayer = layer;
        return result;
    }

    static ImageSubresourceLayers layers(uint32_t base, uint32_t layers) {
        ImageSubresourceLayers result {};
        result.m_description.baseArrayLayer = base;
        result.m_description.layerCount = layers;
        return result;
    }

    static ImageSubresourceLayers single_mip_level(uint32_t level) {
        ImageSubresourceLayers result {};
        result.m_description.mipLevel = level;
        return result;
    }

    const VkImageSubresourceLayers& get_description() const { return m_description; }
    VkImageSubresourceLayers& get_description() { return m_description; }
};

}