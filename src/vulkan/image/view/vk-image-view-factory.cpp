//
// Created by Артем on 27.02.2022.
//

#include "vk-image-view-factory.hpp"

VK::ImageViewFactory::ImageViewFactory() {
    m_description.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    m_description.format = VK_FORMAT_UNDEFINED;
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

VK::ImageView VK::ImageViewFactory::create(VK::Device* device, const VK::ImageBase &image) const {
    m_description.image = image.get_handle();

    VkImageView handle = nullptr;

    if (vkCreateImageView(device->get_handle(), &m_description, nullptr, &handle) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image view");
    }

    return { device, handle };
}
