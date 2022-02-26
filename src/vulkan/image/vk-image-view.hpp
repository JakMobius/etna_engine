#pragma once

#include <vulkan/vulkan_core.h>

#include <utility>
#include "vk-image.hpp"

namespace VK {

// TODO: implement a factory
class ImageView {

    VkImageViewCreateFlags m_flags = 0;
    VkImageViewType m_view_type = VK_IMAGE_VIEW_TYPE_2D;
    VkFormat m_format = VK_FORMAT_MAX_ENUM;
    VkComponentMapping m_components = {
        VK_COMPONENT_SWIZZLE_R,
        VK_COMPONENT_SWIZZLE_G,
        VK_COMPONENT_SWIZZLE_B,
        VK_COMPONENT_SWIZZLE_A
    };
    VkImageSubresourceRange m_subresource_range {};

    VkImageView m_handle = nullptr;
    UnownedImage m_image;

public:

    explicit ImageView(UnownedImage image): m_image(std::move(image)) {
        m_format = VK_FORMAT_MAX_ENUM;
        m_subresource_range.levelCount = 1;
        m_subresource_range.layerCount = 1;
        m_subresource_range.aspectMask = VK_IMAGE_ASPECT_FLAG_BITS_MAX_ENUM;
        m_subresource_range.baseArrayLayer = 0;
        m_subresource_range.baseMipLevel = 0;
    }

    ~ImageView() {
        destroy();
    }

    void create() {
        VkImageViewCreateInfo view_info {};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.image = m_image.get_handle();
        view_info.viewType = m_view_type;
        view_info.format = m_format;
        view_info.subresourceRange = m_subresource_range;

        if (vkCreateImageView(m_image.get_device()->get_handle(), &view_info, nullptr, &m_handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image view");
        }
    }

    void destroy() {
        if(m_handle) {
            vkDestroyImageView(m_image.get_device()->get_handle(), m_handle, nullptr);
            m_handle = nullptr;
        }
    }

    void set_flags(VkImageViewCreateFlags flags) {
        m_flags = flags;
    }

    void set_view_type(VkImageViewType type) {
        m_view_type = type;
    }

    void set_format(VkFormat format) {
        m_format = format;
    }

    void set_components(VkComponentMapping components) {
        m_components = components;
    }

    VkImageViewCreateFlags get_flags() const { return m_flags; }
    VkImageViewType get_view_type() const { return m_view_type; }
    VkFormat get_format() const { return m_format; }
    VkComponentMapping get_components() const { return m_components; }
    VkImageSubresourceRange& get_subresource_range() { return m_subresource_range; }
    VkImageView get_handle() const { return m_handle; }
};

}