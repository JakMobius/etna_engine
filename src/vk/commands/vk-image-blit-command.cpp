
#include <etna/vk-wrappers/commands/vk-image-blit-command.hpp>
#include <etna/vk-wrappers/image/vk-image.hpp>
#include <etna/vk-wrappers/command-buffer/vk-command-buffer.hpp>

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