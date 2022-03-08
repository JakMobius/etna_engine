#pragma once

#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include <etna/vk/device/vk-device.hpp>
#include <etna/vk/image/vk-image.hpp>
#include "vk-image-view.hpp"
#include <etna/vk/image/vk-image-subresource-range.hpp>

namespace VK {

class ImageViewFactory {
    mutable VkImageViewCreateInfo m_description {};
public:

    ImageViewFactory();

    ImageViewFactory& set_flags(VkImageViewCreateFlags flags) {
        m_description.flags = flags;
        return *this;
    }

    ImageViewFactory& set_view_type(VkImageViewType type) {
        m_description.viewType = type;
        return *this;
    }

    ImageViewFactory& set_format(VkFormat format) {
        m_description.format = format;
        return *this;
    }

    ImageViewFactory& set_components(VkComponentMapping components) {
        m_description.components = components;
        return *this;
    }

    ImageView create(Device* device, const ImageBase& image) const;

    VkImageViewCreateFlags get_flags() const { return m_description.flags; }
    VkImageViewType get_view_type() const { return m_description.viewType; }
    VkFormat get_format() const { return m_description.format; }
    VkComponentMapping get_components() const { return m_description.components; }
    ImageSubresourceRange& get_subresource_range() { return ImageSubresourceRange::existing(m_description.subresourceRange); }
    const ImageSubresourceRange& get_subresource_range() const { return ImageSubresourceRange::existing(m_description.subresourceRange); }
};

}