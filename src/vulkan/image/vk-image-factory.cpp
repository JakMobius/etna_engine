//
// Created by Артем on 25.02.2022.
//

#include "vk-image-factory.hpp"

VkImage VK::ImageFactory::create_raw_image(VK::Device* device) {
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

VK::MemoryImage VK::ImageFactory::create(VK::Device* device) {
    Image image {device, create_raw_image(device) };
    MemoryImage memory_image {std::move(image) };
    memory_image.create(m_memory_properties);
    return memory_image;
}
