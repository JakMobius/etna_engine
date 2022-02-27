#pragma once

#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include "../../device/vk-device.hpp"
#include "../vk-image.hpp"
#include "vk-image-view.hpp"

namespace VK {

class ImageViewFactory {
    VkImageViewCreateInfo m_description {};
public:

    ImageViewFactory() {
        m_description.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        m_description.format = VK_FORMAT_MAX_ENUM;
        m_description.subresourceRange.levelCount = 1;
        m_description.subresourceRange.layerCount = 1;
        m_description.subresourceRange.aspectMask = VK_IMAGE_ASPECT_FLAG_BITS_MAX_ENUM;
        m_description.subresourceRange.baseArrayLayer = 0;
        m_description.subresourceRange.baseMipLevel = 0;
        m_description.viewType = VK_IMAGE_VIEW_TYPE_2D;
        m_description.format = VK_FORMAT_MAX_ENUM;
        m_description.components = {
            VK_COMPONENT_SWIZZLE_R,
            VK_COMPONENT_SWIZZLE_G,
            VK_COMPONENT_SWIZZLE_B,
            VK_COMPONENT_SWIZZLE_A
        };
    }

    void set_flags(VkImageViewCreateFlags flags) { m_description.flags = flags;}
    void set_view_type(VkImageViewType type) { m_description.viewType = type; }
    void set_format(VkFormat format) { m_description.format = format; }
    void set_components(VkComponentMapping components) { m_description.components = components; }

    ImageView create(Device* device, const ImageBase& image) {
        m_description.image = image.get_handle();

        VkImageView handle = nullptr;

        if (vkCreateImageView(device->get_handle(), &m_description, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image view");
        }

        return { device, handle };
    }

    VkImageViewCreateFlags get_flags() const { return m_description.flags; }
    VkImageViewType get_view_type() const { return m_description.viewType; }
    VkFormat get_format() const { return m_description.format; }
    VkComponentMapping get_components() const { return m_description.components; }
    VkImageSubresourceRange& get_subresource_range() { return m_description.subresourceRange; }
};

}