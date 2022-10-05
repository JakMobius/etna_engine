#include <etna/vk-wrappers/commands/vk-copy-image-command.hpp>

void VK::CopyImageCommand::write(const VK::UnownedCommandBuffer &command_buffer) {
    vkCmdCopyImage(command_buffer.get_handle(),
                   m_source.get_handle(),
                   m_source_layout,
                   m_destination.get_handle(),
                   m_destination_layout,
                   1,
                   &m_region);
}
