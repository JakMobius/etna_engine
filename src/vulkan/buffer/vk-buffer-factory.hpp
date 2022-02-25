#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "../vk-device.hpp"
#include "../vk-memory.hpp"
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

    void set_size(VkDeviceSize size) {
        m_create_info.size = size;
    }

    void set_usage(VkBufferUsageFlags usage) {
        m_create_info.usage = usage;
    }

    void set_memory_properties(VkMemoryPropertyFlags properties) {
        m_memory_properties = properties;
    }

    void set_flags(VkBufferCreateFlags flags) {
        m_create_info.flags = flags;
    }

    std::vector<uint32_t>& get_queue_families() {
        return m_queue_families;
    }

    VkBuffer create_raw_buffer(Device* device) {

        m_create_info.pQueueFamilyIndices = m_queue_families.data();
        m_create_info.queueFamilyIndexCount = m_queue_families.size();

        VkBuffer buffer = nullptr;

        if (vkCreateBuffer(device->get_handle(), &m_create_info, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer");
        }

        return buffer;
    }

    MemoryBuffer create_memory_buffer(Device* device) {
        auto physical_device = device->get_physical_device();

        VkBuffer raw_buffer = create_raw_buffer(device);
        VK::Buffer buffer { device, raw_buffer };

        VkMemoryRequirements mem_requirements {};
        vkGetBufferMemoryRequirements(device->get_handle(), buffer.get_handle(), &mem_requirements);
        auto memory_type = physical_device->get_suitable_memory_type(mem_requirements.memoryTypeBits, m_memory_properties);

        auto result = MemoryBuffer { std::move(buffer) };
        result.create(memory_type);
        return result;
    }

    VkDeviceSize get_size() const { return m_create_info.size; }
    VkBufferUsageFlags get_usage() const { return m_create_info.usage; }
    VkBufferUsageFlags get_flags() const { return m_create_info.flags; }
    const VkMemoryPropertyFlags& get_set_memory_properties() const { return m_memory_properties; }
};

}