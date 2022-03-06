#pragma once

#include <vulkan/vulkan_core.h>
#include "../vk-memory.hpp"
#include "vk-memory-image.hpp"

namespace VK {

class ImageFactory {

    VkImageCreateInfo m_description {};
    VkMemoryPropertyFlags m_memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

public:
    ImageFactory();

    void set_flags(VkImageCreateFlags flags) {
        m_description.flags = flags;
    }

    void set_format(VkFormat format) {
        m_description.format = format;
    }

    void set_mip_levels(uint32_t mip_levels) {
        m_description.mipLevels = mip_levels;
    }

    void set_array_layers(uint32_t array_layers) {
        m_description.arrayLayers = array_layers;
    }

    void set_samples(VkSampleCountFlagBits samples) {
        m_description.samples = samples;
    }

    void set_tiling(VkImageTiling tiling) {
        m_description.tiling = tiling;
    }

    void set_usage(VkBufferUsageFlags usage) {
        m_description.usage = usage;
    }

    void set_initial_layout(VkImageLayout initial_layout) {
        m_description.initialLayout = initial_layout;
    }

    void set_sharing_mode(VkSharingMode sharing_mode) {
        m_description.sharingMode = sharing_mode;
    }

    void set_memory_properties(VkMemoryPropertyFlags memory_properties) {
        m_memory_properties = memory_properties;
    }

    void set_image_type(VkImageType image_type) {
        m_description.imageType = image_type;
    }

    void set_extent(VkExtent3D extent) {
        m_description.extent = extent;
    }

    VkImageType get_image_type()                  const { return m_description.imageType; }
    VkImageCreateFlags get_flags()                const { return m_description.flags; }
    VkFormat get_format()                         const { return m_description.format; }
    uint32_t get_mip_levels()                     const { return m_description.mipLevels; }
    uint32_t get_array_layers()                   const { return m_description.arrayLayers; }
    VkSampleCountFlagBits get_samples()           const { return m_description.samples; }
    VkImageTiling get_tiling()                    const { return m_description.tiling; }
    VkBufferUsageFlags get_usage()                const { return m_description.flags; }
    VkImageLayout get_initial_layout()            const { return m_description.initialLayout; }
    VkSharingMode get_sharing_mode()              const { return m_description.sharingMode; }
    VkMemoryPropertyFlags get_memory_properties() const { return m_memory_properties; }
    VkExtent3D get_extent()                       const { return m_description.extent; }

    VkImage create_raw_image(Device* device) const;

    MemoryImage create(Device* device) const;
};

}