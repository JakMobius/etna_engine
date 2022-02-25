#pragma once

#include <vulkan/vulkan_core.h>
#include "../buffer/vk-buffer.hpp"
#include "vk-command.hpp"

namespace VK {

class CopyBufferCommand : public Command {
    VkDeviceSize m_src_offset = 0;
    VkDeviceSize m_dst_offset = 0;
    VkDeviceSize m_size = 0;
    VK::Buffer* m_source;
    VK::Buffer* m_destination;
public:
    CopyBufferCommand(VK::Buffer* source, VK::Buffer* destination): m_source(source), m_destination(destination) {}

    void set_src_offset(VkDeviceSize src_offset) { m_src_offset = src_offset; }
    void set_dst_offset(VkDeviceSize dst_offset) { m_dst_offset = dst_offset; }
    void set_size(VkDeviceSize size) { m_size = size; }

    VkDeviceSize get_src_offset() const { return m_src_offset; }
    VkDeviceSize get_dst_offset() const { return m_dst_offset; }
    VkDeviceSize get_size() const { return m_size; }

    void write(VK::CommandBuffer* command_buffer) override {

        VkBufferCopy copy_region {};
        copy_region.srcOffset = m_src_offset;
        copy_region.dstOffset = m_dst_offset;
        copy_region.size = m_size;

        vkCmdCopyBuffer(command_buffer->get_handle(), m_source->get_handle(), m_destination->get_handle(), 1, &copy_region);
    }


};

}