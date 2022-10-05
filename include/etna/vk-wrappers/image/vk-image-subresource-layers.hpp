#pragma once

#include <etna/volk.hpp>

namespace VK {

class ImageSubresourceLayers : public VkImageSubresourceLayers {
public:
    ImageSubresourceLayers(): VkImageSubresourceLayers() {
        baseArrayLayer = 0;
        layerCount = 1;
        mipLevel = 0;
        aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }
    ImageSubresourceLayers(const ImageSubresourceLayers& copy) = default;
    ImageSubresourceLayers(ImageSubresourceLayers&& move) noexcept = default;
    ImageSubresourceLayers& operator=(const ImageSubresourceLayers& copy_assign) = default;
    ImageSubresourceLayers& operator=(ImageSubresourceLayers&& move_assign) noexcept = default;

    static ImageSubresourceLayers& existing(VkImageSubresourceLayers& range) {
        return reinterpret_cast<ImageSubresourceLayers&>(range);
    }

    static const ImageSubresourceLayers& existing(const VkImageSubresourceLayers& range) {
        return reinterpret_cast<const ImageSubresourceLayers&>(range);
    }

    uint32_t get_mip_level()            const { return mipLevel; }
    uint32_t get_array_layers()          const { return layerCount; }
    uint32_t get_base_array_layer()      const { return baseArrayLayer; }
    VkImageAspectFlags get_aspect_mask() const { return aspectMask; }

    ImageSubresourceLayers& set_mip_level(uint32_t base_mip_level) {
        mipLevel = base_mip_level;
        return *this;
    }

    ImageSubresourceLayers& set_array_layers(uint32_t array_layers) {
        layerCount = array_layers;
        return *this;
    }

    ImageSubresourceLayers& set_base_array_layer(uint32_t base_array_layer) {
        baseArrayLayer = base_array_layer;
        return *this;
    }

    ImageSubresourceLayers& set_aspect_mask(VkImageAspectFlags mask) {
        aspectMask = mask;
        return *this;
    }

    static ImageSubresourceLayers single_layer(uint32_t layer) {
        ImageSubresourceLayers result {};
        result.baseArrayLayer = layer;
        return result;
    }

    static ImageSubresourceLayers layers(uint32_t base, uint32_t layers) {
        ImageSubresourceLayers result {};
        result.baseArrayLayer = base;
        result.layerCount = layers;
        return result;
    }

    static ImageSubresourceLayers single_mip_level(uint32_t level) {
        ImageSubresourceLayers result {};
        result.mipLevel = level;
        return result;
    }
};

}