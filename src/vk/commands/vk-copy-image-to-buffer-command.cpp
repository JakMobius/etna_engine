#include <etna/vk-wrappers/commands/vk-copy-image-to-buffer-command.hpp>

VK::CopyImageToBufferCommand::CopyImageToBufferCommand(const VK::UnownedImage& image, const VK::UnownedBuffer& buffer) : m_buffer(buffer), m_image(image) {

    m_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_region.imageSubresource.layerCount = 1;
    m_region.imageExtent = { 0, 0, 1 };
}

void VK::CopyImageToBufferCommand::write(const VK::UnownedCommandBuffer& command_buffer) {
    vkCmdCopyImageToBuffer(
            command_buffer.get_handle(),
            m_image.get_handle(),
            m_src_image_layout,
            m_buffer.get_handle(),
            1,
            &m_region
    );
}