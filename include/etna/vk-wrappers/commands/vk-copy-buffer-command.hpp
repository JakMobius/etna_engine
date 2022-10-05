#pragma once

namespace VK {
class Buffer;
}

#include <etna/volk.hpp>
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

    CopyBufferCommand& set_src_offset(VkDeviceSize src_offset) {
        m_src_offset = src_offset;
        return *this;
    }

    CopyBufferCommand& set_dst_offset(VkDeviceSize dst_offset) {
        m_dst_offset = dst_offset;
        return *this;
    }

    CopyBufferCommand& set_size(VkDeviceSize size) {
        m_size = size;
        return *this;
    }

    VkDeviceSize get_src_offset() const { return m_src_offset; }
    VkDeviceSize get_dst_offset() const { return m_dst_offset; }
    VkDeviceSize get_size() const { return m_size; }

    void write(const UnownedCommandBuffer& command_buffer) override;


};

}