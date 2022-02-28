//
// Created by Артем on 16.02.2022.
//

#include "vk-copy-buffer-to-image-command.hpp"
#include "../buffer/vk-buffer.hpp"
#include "../image/vk-image.hpp"
#include "../command-buffer/vk-command-buffer.hpp"

VK::CopyBufferToImageCommand::CopyBufferToImageCommand(VK::Buffer* buffer, VK::Image* image) : m_buffer(buffer), m_image(image) {

    m_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_region.imageSubresource.layerCount = 1;
    m_region.imageExtent = { 0, 0, 1 };
}

void VK::CopyBufferToImageCommand::write(VK::CommandBuffer* command_buffer) {
    vkCmdCopyBufferToImage(
            command_buffer->get_handle(),
            m_buffer->get_handle(),
            m_image->get_handle(),
            m_dst_image_layout,
            1,
            &m_region
    );
}
