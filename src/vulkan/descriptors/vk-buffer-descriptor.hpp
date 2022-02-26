#pragma once

#include "vk-descriptor.hpp"
#include "../buffer/vk-buffer.hpp"

namespace VK {

class BufferDescriptor : public Descriptor {
    VkDescriptorBufferInfo m_buffer_info {};
public:
    BufferDescriptor(const Buffer& buffer, VkDeviceSize offset, VkDeviceSize size): Descriptor() {
        m_description.dstArrayElement = 0;
        m_description.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        m_description.descriptorCount = 1;
        m_description.pBufferInfo = &m_buffer_info;

        m_buffer_info.buffer = buffer.get_handle();
        m_buffer_info.offset = offset;
        m_buffer_info.range = size;
    }
};

}