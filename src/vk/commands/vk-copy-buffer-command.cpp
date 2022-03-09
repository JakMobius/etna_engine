
#include <etna/vk-wrappers/commands/vk-copy-buffer-command.hpp>
#include <etna/vk-wrappers/buffer/vk-buffer.hpp>
#include <etna/vk-wrappers/command-buffer/vk-command-buffer.hpp>

void VK::CopyBufferCommand::write(const UnownedCommandBuffer& command_buffer) {

    VkBufferCopy copy_region {};
    copy_region.srcOffset = m_src_offset;
    copy_region.dstOffset = m_dst_offset;
    copy_region.size = m_size;

    vkCmdCopyBuffer(command_buffer.get_handle(), m_source->get_handle(), m_destination->get_handle(), 1, &copy_region);
}
