#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-base-image.hpp"

namespace VK {

class ImageView {

    VkImageViewCreateFlags m_flags = 0;
    VkImageViewType m_view_type = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
    VkFormat m_format = VK_FORMAT_MAX_ENUM;
    VkComponentMapping m_components = {
        VK_COMPONENT_SWIZZLE_R,
        VK_COMPONENT_SWIZZLE_G,
        VK_COMPONENT_SWIZZLE_B,
        VK_COMPONENT_SWIZZLE_A
    };
    VkImageSubresourceRange m_subresource_range {};

    VkImageView m_handle = nullptr;

    VkImage m_image_handle;
    VK::Device* m_device = nullptr;

public:

    explicit ImageView(BaseImage* image):
            m_image_handle(image->get_handle()),
            m_device(image->get_memory()->get_device()){
        auto type = image->get_image_type();

        switch(type) {
            case VK_IMAGE_TYPE_1D: m_view_type = VK_IMAGE_VIEW_TYPE_1D; break;
            case VK_IMAGE_TYPE_2D: m_view_type = VK_IMAGE_VIEW_TYPE_2D; break;
            case VK_IMAGE_TYPE_3D: m_view_type = VK_IMAGE_VIEW_TYPE_3D; break;
            default: break;
        }

        m_format = image->get_format();
        m_subresource_range.levelCount = image->get_mip_levels();
        m_subresource_range.layerCount = image->get_array_layers();
        m_subresource_range.aspectMask = VK_IMAGE_ASPECT_FLAG_BITS_MAX_ENUM;
        m_subresource_range.baseArrayLayer = 0;
        m_subresource_range.baseMipLevel = 0;
    }

    explicit ImageView(VkImage image, VK::Device* device): m_image_handle(image), m_device(device) {
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
        view_info.image = m_image_handle;
        view_info.viewType = m_view_type;
        view_info.format = m_format;
        view_info.subresourceRange = m_subresource_range;

        if (vkCreateImageView(m_device->get_handle(), &view_info, nullptr, &m_handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image view");
        }
    }

    void destroy() {
        if(m_handle) {
            vkDestroyImageView(m_device->get_handle(), m_handle, nullptr);
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

    VkImageViewCreateFlags get_flags() { return m_flags; }
    VkImageViewType get_view_type() { return m_view_type; }
    VkFormat get_format() { return m_format; }
    VkComponentMapping get_components() { return m_components; }
    VkImageSubresourceRange& get_subresource_range() { return m_subresource_range; }
    VkImage get_image_handle() { return m_image_handle; }
    VkImageView get_handle() { return m_handle; }
};

}