#pragma once

namespace VK {
class Buffer;
}

#include <vulkan/vulkan_core.h>
#include "vk-command.hpp"

namespace VK {

class CopyBufferCommand : public Command {
    VkDeviceSize m_src_offset = 0;
    VkDeviceSize m_dst_offset = 0;
    VkDeviceSize m_size = 0;
    Buffer* m_source;
    Buffer* m_destination;
public:
    CopyBufferCommand(Buffer* source, Buffer* destination): m_source(source), m_destination(destination) {}

    void set_src_offset(VkDeviceSize src_offset) { m_src_offset = src_offset; }
    void set_dst_offset(VkDeviceSize dst_offset) { m_dst_offset = dst_offset; }
    void set_size(VkDeviceSize size) { m_size = size; }

    VkDeviceSize get_src_offset() const { return m_src_offset; }
    VkDeviceSize get_dst_offset() const { return m_dst_offset; }
    VkDeviceSize get_size() const { return m_size; }

    void write(VK::CommandBuffer* command_buffer) override;


};

}