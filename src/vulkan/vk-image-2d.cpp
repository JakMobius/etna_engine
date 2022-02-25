//
// Created by Артем on 14.02.2022.
//

#include "vk-image-2d.hpp"
#include "barriers/vk-image-memory-barrier.hpp"

void VK::Image2D::create() {

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

    auto device = m_memory->get_device();

    if (vkCreateImage(device->get_handle(), &image_info, nullptr, &m_handle) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements mem_requirements {};
    vkGetImageMemoryRequirements(device->get_handle(), m_handle, &mem_requirements);

    auto mem_type = device->get_physical_device()->get_suitable_memory_type(mem_requirements.memoryTypeBits, m_memory_properties);

    m_memory->set_device(device);
    m_memory->set_size(mem_requirements.size);
    m_memory->set_type(mem_type);
    m_memory->allocate();

    vkBindImageMemory(device->get_handle(), m_handle, m_memory->get_handle(), 0);
}

