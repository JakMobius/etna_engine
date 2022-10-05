#pragma once

#include <etna/volk.hpp>
#include <etna/vk-wrappers/vk-memory.hpp>
#include "vk-memory-image.hpp"

namespace VK {

class ImageFactory {

    VkImageCreateInfo m_description {};
    VkMemoryPropertyFlags m_memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

public:
    ImageFactory();

    ImageFactory& set_flags(VkImageCreateFlags flags) {
        m_description.flags = flags;
        return *this;
    }

    ImageFactory& set_format(VkFormat format) {
        m_description.format = format;
        return *this;
    }

    ImageFactory& set_mip_levels(uint32_t mip_levels) {
        m_description.mipLevels = mip_levels;
        return *this;
    }

    ImageFactory& set_array_layers(uint32_t array_layers) {
        m_description.arrayLayers = array_layers;
        return *this;
    }

    ImageFactory& set_samples(VkSampleCountFlagBits samples) {
        m_description.samples = samples;
        return *this;
    }

    ImageFactory& set_tiling(VkImageTiling tiling) {
        m_description.tiling = tiling;
        return *this;
    }

    ImageFactory& set_usage(VkBufferUsageFlags usage) {
        m_description.usage = usage;
        return *this;
    }

    ImageFactory& set_initial_layout(VkImageLayout initial_layout) {
        m_description.initialLayout = initial_layout;
        return *this;
    }

    ImageFactory& set_sharing_mode(VkSharingMode sharing_mode) {
        m_description.sharingMode = sharing_mode;
        return *this;
    }

    ImageFactory& set_memory_properties(VkMemoryPropertyFlags memory_properties) {
        m_memory_properties = memory_properties;
        return *this;
    }

    ImageFactory& set_image_type(VkImageType image_type) {
        m_description.imageType = image_type;
        return *this;
    }

    ImageFactory& set_extent(VkExtent3D extent) {
        m_description.extent = extent;
        return *this;
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