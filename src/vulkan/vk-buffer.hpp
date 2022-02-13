#pragma once

#include <exception>
#include "vk-memory.hpp"

namespace VK {

class Buffer {
    MemoryReference m_memory_ref {};
    VkDeviceSize m_size = 0;
    VkBufferUsageFlags m_usage = 0;
    VkMemoryPropertyFlags m_properties = 0;
    VkSharingMode m_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
    VkBuffer m_handle = nullptr;

    static void report_illegal_state_change() {
        throw std::runtime_error("cannot change VK::Buffer internal state while it is created");
    }

public:

    explicit Buffer(const MemoryReference& reference): m_memory_ref(reference) {}
    ~Buffer() {
        destroy();
    }

    VkBuffer get_handle() {
        return m_handle;
    }

    void set_size(VkDeviceSize size) {
        if(m_handle) report_illegal_state_change();
        m_size = size;
    }

    void set_usage(VkBufferUsageFlags usage) {
        if(m_handle) report_illegal_state_change();
        m_usage = usage;
    }

    void set_properties(VkMemoryPropertyFlags properties) {
        if(m_handle) report_illegal_state_change();
        m_properties = properties;
    }

    void create() {
        VkBufferCreateInfo buffer_info{};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = m_size;
        buffer_info.usage = m_usage;
        buffer_info.sharingMode = m_sharing_mode;

        auto memory = m_memory_ref.get_memory();
        auto device = memory->get_device();

        if (vkCreateBuffer(device->get_handle(), &buffer_info, nullptr, &m_handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer");
        }

        VkMemoryRequirements mem_requirements {};
        vkGetBufferMemoryRequirements(device->get_handle(), m_handle, &mem_requirements);

        memory->set_size(mem_requirements.size);
        memory->set_type(memory->get_suitable_memory_type(mem_requirements.memoryTypeBits, m_properties));
        memory->allocate();

        vkBindBufferMemory(device->get_handle(), m_handle, memory->get_handle(), 0);
    }

    void destroy() {
        if(!m_handle) return;

        auto device = m_memory_ref.get_memory()->get_device();
        vkDestroyBuffer(device->get_handle(), m_handle, nullptr);
        m_handle = nullptr;
    }
};

}