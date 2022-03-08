#pragma once

#include "vk-descriptor.hpp"
#include <etna/vk/buffer/vk-buffer.hpp>

namespace VK {

class BufferDescriptor : public Descriptor {
    VkDescriptorBufferInfo m_buffer_info {};
public:
    BufferDescriptor(const Buffer& buffer, VkDeviceSize offset, VkDeviceSize size);
};

}