#pragma once

#include "vk-descriptor.hpp"
#include <etna/vk-wrappers/buffer/vk-buffer.hpp>

namespace VK {

class UniformBufferDescriptor : public Descriptor {
    VkDescriptorBufferInfo m_buffer_info {};
public:
    UniformBufferDescriptor(const Buffer& buffer, VkDeviceSize offset, VkDeviceSize size);
};

}