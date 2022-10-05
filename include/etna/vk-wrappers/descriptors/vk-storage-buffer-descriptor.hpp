#pragma once

#include "vk-descriptor.hpp"
#include <etna/vk-wrappers/buffer/vk-buffer.hpp>

namespace VK {

class StorageBufferDescriptor : public Descriptor {
    VkDescriptorBufferInfo m_buffer_info {};
public:
    StorageBufferDescriptor(const Buffer& buffer, VkDeviceSize offset, VkDeviceSize size);
};

}