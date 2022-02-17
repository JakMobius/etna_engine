#pragma once

#include "vk-memory-buffer.hpp"

namespace VK {

class StagingBuffer : public MemoryBuffer {
public:

    template<typename T>
    StagingBuffer(Device* device, std::vector<T> data): MemoryBuffer(device) {
        VkDeviceSize buffer_size = sizeof(data[0]) * data.size();

        m_buffer.set_usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        m_buffer.set_properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        m_buffer.set_size(buffer_size);
        m_buffer.create();
        m_memory.set_data(data.data(), (size_t) buffer_size);
    }

    StagingBuffer(Device* device, void* data, size_t size): MemoryBuffer(device) {
        m_buffer.set_usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        m_buffer.set_properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        m_buffer.set_size(size);
        m_buffer.create();
        m_memory.set_data(data, size);
    }
};

}