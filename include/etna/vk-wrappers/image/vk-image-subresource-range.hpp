#pragma once

#include <etna/volk.hpp>

namespace VK {

class ImageSubresourceRange : public VkImageSubresourceRange {
public:
    ImageSubresourceRange() {
        baseArrayLayer = 0;
        levelCount = 1;
        baseMipLevel = 0;
        layerCount = 1;
        aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }
    ImageSubresourceRange(const ImageSubresourceRange& copy) = default;
    ImageSubresourceRange(ImageSubresourceRange&& move) noexcept = default;
    ImageSubresourceRange& operator=(const ImageSubresourceRange& copy_assign) = default;
    ImageSubresourceRange& operator=(ImageSubresourceRange&& move_assign) noexcept = default;

    static ImageSubresourceRange& existing(VkImageSubresourceRange& range) {
        return reinterpret_cast<ImageSubresourceRange&>(range);
    }

    static const ImageSubresourceRange& existing(const VkImageSubresourceRange& range) {
        return reinterpret_cast<const ImageSubresourceRange&>(range);
    }

    uint32_t get_mip_levels()            const { return levelCount; }
    uint32_t get_base_mip_level()        const { return baseMipLevel; }
    uint32_t get_array_layers()          const { return layerCount; }
    uint32_t get_base_array_layer()      const { return baseArrayLayer; }
    VkImageAspectFlags get_aspect_mask() const { return aspectMask; }

    ImageSubresourceRange& set_mip_levels(uint32_t mip_levels) {
        levelCount = mip_levels;
        return *this;
    }

    ImageSubresourceRange& set_base_mip_level(uint32_t base_mip_level) {
        baseMipLevel = base_mip_level;
        return *this;
    }

    ImageSubresourceRange& set_array_layers(uint32_t array_layers) {
        layerCount = array_layers;
        return *this;
    }

    ImageSubresourceRange& set_base_array_layer(uint32_t base_array_layer) {
        baseArrayLayer = base_array_layer;
        return *this;
    }

    ImageSubresourceRange& set_aspect_mask(VkImageAspectFlags mask) {
        aspectMask = mask;
        return *this;
    }

    static ImageSubresourceRange single_mip_level(uint32_t level) {
        ImageSubresourceRange result {};
        result.baseMipLevel = level;
        return result;
    }

    static ImageSubresourceRange mip_levels(uint32_t count) {
        ImageSubresourceRange result {};
        result.levelCount = count;
        return result;
    }

    static ImageSubresourceRange mip_levels(uint32_t base, uint32_t count) {
        ImageSubresourceRange result {};
        result.baseMipLevel = base;
        result.levelCount = count;
        return result;
    }
};

}