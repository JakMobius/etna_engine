//
// Created by Артем on 23.02.2022.
//

#include "vk-image-blit-command.hpp"
#include "../image/vk-image.hpp"
#include "../command-buffer/vk-command-buffer.hpp"

VK::ImageBlitCommand::ImageBlitCommand(const VK::UnownedImage& source, const VK::UnownedImage& destination) : m_source(source), m_destination(destination) {
    get_source_subresource_layers() = ImageSubresourceLayers();
    get_destination_subresource_layers() = ImageSubresourceLayers();
}

void VK::ImageBlitCommand::write(const UnownedCommandBuffer& command_buffer) {

    vkCmdBlitImage(command_buffer.get_handle(),
                   m_source.get_handle(), m_source_layout,
                   m_destination.get_handle(), m_destination_layout,
                   1, &m_description,
                   m_filter);
}