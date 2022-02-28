//
// Created by Артем on 23.02.2022.
//

#include "vk-image-blit-command.hpp"
#include "../image/vk-image.hpp"
#include "../command-buffer/vk-command-buffer.hpp"

VK::ImageBlitCommand::ImageBlitCommand(VK::Image* source, VK::Image* destination) : m_source(source), m_destination(destination) {
    m_description.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_description.srcSubresource.mipLevel = 0;
    m_description.srcSubresource.baseArrayLayer = 0;
    m_description.srcSubresource.layerCount = 1;

    m_description.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_description.dstSubresource.mipLevel = 0;
    m_description.dstSubresource.baseArrayLayer = 0;
    m_description.dstSubresource.layerCount = 1;
}

void VK::ImageBlitCommand::write(VK::CommandBuffer* command_buffer) {

    vkCmdBlitImage(command_buffer->get_handle(),
                   m_source->get_handle(), m_source_layout,
                   m_destination->get_handle(), m_destination_layout,
                   1, &m_description,
                   m_filter);
}

void VK::ImageBlitCommand::setup_mip_offsets(int32_t mip_width, int32_t mip_height) {
    m_description.srcOffsets[0] = { 0, 0, 0 };
    m_description.srcOffsets[1] = { mip_width, mip_height, 1 };

    m_description.dstOffsets[0] = { 0, 0, 0 };
    m_description.dstOffsets[1] = { mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1 };
}
