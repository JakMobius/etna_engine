//
// Created by Артем on 14.02.2022.
//

#include "vk-image-2d.hpp"

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

void VK::Image2D::perform_layout_transition(VK::CommandBuffer* command_buffer, VkImageLayout old_layout,
                                            VkImageLayout new_layout) {
    VkImageMemoryBarrier barrier {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;

    barrier.image = m_handle;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = m_mip_levels;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;

    VkPipelineStageFlags source_stage = 0;
    VkPipelineStageFlags destination_stage = 0;

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    } else {
        throw std::invalid_argument("unsupported layout transition");
    }

    vkCmdPipelineBarrier(
            command_buffer->get_handle(),
            source_stage, destination_stage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
    );
}
