#pragma once

#include <vulkan/vulkan_core.h>
#include "../vk-memory.hpp"
#include "vk-memory-image.hpp"

namespace VK {

class ImageFactory {

    VkExtent3D m_extent = {0, 0, 0};
    VkImageCreateFlags m_flags = 0;
    VkFormat m_format = VK_FORMAT_MAX_ENUM;
    uint32_t m_mip_levels = 1;
    uint32_t m_array_layers = 1;
    VkSampleCountFlagBits m_samples = VK_SAMPLE_COUNT_1_BIT;
    VkImageTiling m_tiling = VK_IMAGE_TILING_OPTIMAL;
    VkImageUsageFlags m_usage = 0;
    VkImageLayout m_initial_layout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkSharingMode m_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
    VkImageType m_image_type = VK_IMAGE_TYPE_2D;
    VkMemoryPropertyFlags m_memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

public:
    ImageFactory() = default;

    void set_flags(VkImageCreateFlags flags) {
        m_flags = flags;
    }

    void set_format(VkFormat format) {
        m_format = format;
    }

    void set_mip_levels(uint32_t mip_levels) {
        m_mip_levels = mip_levels;
    }

    void set_array_layers(uint32_t array_layers) {
        m_array_layers = array_layers;
    }

    void set_samples(VkSampleCountFlagBits samples) {
        m_samples = samples;
    }

    void set_tiling(VkImageTiling tiling) {
        m_tiling = tiling;
    }

    void set_usage(VkBufferUsageFlags usage) {
        m_usage = usage;
    }

    void set_initial_layout(VkImageLayout initial_layout) {
        m_initial_layout = initial_layout;
    }

    void set_sharing_mode(VkSharingMode sharing_mode) {
        m_sharing_mode = sharing_mode;
    }

    void set_memory_properties(VkMemoryPropertyFlags memory_properties) {
        m_memory_properties = memory_properties;
    }

    void set_image_type(VkImageType image_type) {
        m_image_type = image_type;
    }

    void set_extent(VkExtent3D extent) {
        m_extent = extent;
    }

    VkImageType get_image_type()                  const { return m_image_type; }
    VkImageCreateFlags get_flags()                const { return m_flags; }
    VkFormat get_format()                         const { return m_format; }
    uint32_t get_mip_levels()                     const { return m_mip_levels; }
    uint32_t get_array_layers()                   const { return m_array_layers; }
    VkSampleCountFlagBits get_samples()           const { return m_samples; }
    VkImageTiling get_tiling()                    const { return m_tiling; }
    VkBufferUsageFlags get_usage()                const { return m_flags; }
    VkImageLayout get_initial_layout()            const { return m_initial_layout; }
    VkSharingMode get_sharing_mode()              const { return m_sharing_mode; }
    VkMemoryPropertyFlags get_memory_properties() const { return m_memory_properties; }
    VkExtent3D get_extent()                       const { return m_extent; }

    VkImage create_raw_image(Device* device) {
        VkImageCreateInfo image_info {};
        image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_info.imageType = VK_IMAGE_TYPE_2D;
        image_info.extent = m_extent;
        image_info.mipLevels = m_mip_levels;
        image_info.arrayLayers = m_array_layers;
        image_info.format = m_format;
        image_info.tiling = m_tiling;
        image_info.initialLayout = m_initial_layout;
        image_info.usage = m_usage;
        image_info.samples = m_samples;
        image_info.sharingMode = m_sharing_mode;

        VkImage handle = nullptr;

        if (vkCreateImage(device->get_handle(), &image_info, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        return handle;
    }

    MemoryImage create(Device* device) {
        Image image { device, create_raw_image(device) };
        MemoryImage memory_image {std::move(image) };
        memory_image.create(m_memory_properties);
        return memory_image;
    }
};

}