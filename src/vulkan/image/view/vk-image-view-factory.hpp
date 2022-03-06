#pragma once

#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include "../../device/vk-device.hpp"
#include "../vk-image.hpp"
#include "vk-image-view.hpp"
#include "../vk-image-subresource-range.hpp"

namespace VK {

class ImageViewFactory {
    mutable VkImageViewCreateInfo m_description {};
public:

    ImageViewFactory();

    void set_flags(VkImageViewCreateFlags flags) { m_description.flags = flags;}
    void set_view_type(VkImageViewType type) { m_description.viewType = type; }
    void set_format(VkFormat format) { m_description.format = format; }
    void set_components(VkComponentMapping components) { m_description.components = components; }

    ImageView create(Device* device, const ImageBase& image) const;

    VkImageViewCreateFlags get_flags() const { return m_description.flags; }
    VkImageViewType get_view_type() const { return m_description.viewType; }
    VkFormat get_format() const { return m_description.format; }
    VkComponentMapping get_components() const { return m_description.components; }
    ImageSubresourceRange& get_subresource_range() { return ImageSubresourceRange::existing(m_description.subresourceRange); }
    const ImageSubresourceRange& get_subresource_range() const { return ImageSubresourceRange::existing(m_description.subresourceRange); }
};

}