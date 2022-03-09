
#include <etna/vk-wrappers/commands/vk-copy-buffer-to-image-command.hpp>
#include <etna/vk-wrappers/buffer/vk-buffer.hpp>
#include <etna/vk-wrappers/image/vk-image.hpp>
#include <etna/vk-wrappers/command-buffer/vk-command-buffer.hpp>

VK::CopyBufferToImageCommand::CopyBufferToImageCommand(const VK::UnownedBuffer& buffer, const VK::UnownedImage& image) : m_buffer(buffer), m_image(image) {

    m_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_region.imageSubresource.layerCount = 1;
    m_region.imageExtent = { 0, 0, 1 };
}

void VK::CopyBufferToImageCommand::write(const VK::UnownedCommandBuffer& command_buffer) {
    vkCmdCopyBufferToImage(
            command_buffer.get_handle(),
            m_buffer.get_handle(),
            m_image.get_handle(),
            m_dst_image_layout,
            1,
            &m_region
    );
}
