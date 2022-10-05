#pragma once

#include <vector>
#include <etna/volk.hpp>
#include "vk-memory-buffer.hpp"

namespace VK {

class BufferFactory {
    VkBufferCreateInfo m_create_info {};
    VkMemoryPropertyFlags m_memory_properties = 0;
    std::vector<uint32_t> m_queue_families {};

public:
    BufferFactory() {
        m_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    }

    BufferFactory& set_size(VkDeviceSize size) {
        m_create_info.size = size;
        return *this;
    }

    BufferFactory& set_usage(VkBufferUsageFlags usage) {
        m_create_info.usage = usage;
        return *this;
    }

    BufferFactory& set_memory_properties(VkMemoryPropertyFlags properties) {
        m_memory_properties = properties;
        return *this;
    }

    BufferFactory& set_flags(VkBufferCreateFlags flags) {
        m_create_info.flags = flags;
        return *this;
    }

    std::vector<uint32_t>& get_queue_families() {
        return m_queue_families;
    }

    VkBuffer create_raw_buffer(Device* device);

    MemoryBuffer create_memory_buffer(Device* device);

    VkDeviceSize get_size() const { return m_create_info.size; }
    VkBufferUsageFlags get_usage() const { return m_create_info.usage; }
    VkBufferUsageFlags get_flags() const { return m_create_info.flags; }
    const VkMemoryPropertyFlags& get_set_memory_properties() const { return m_memory_properties; }
};

}