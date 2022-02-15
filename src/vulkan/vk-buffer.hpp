#pragma once

#include <exception>
#include "vk-memory.hpp"
#include "vk-command-buffer.hpp"

namespace VK {

class Buffer {
    Memory* m_memory {};
    VkDeviceSize m_size = 0;
    VkBufferUsageFlags m_usage = 0;
    VkMemoryPropertyFlags m_properties = 0;
    VkSharingMode m_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
    VkBuffer m_handle = nullptr;

    static void report_illegal_state_change() {
        throw std::runtime_error("cannot change VK::Buffer internal state while it is created");
    }

public:

    explicit Buffer(Memory* memory): m_memory(memory) {}
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

        auto device = m_memory->get_device();
        auto physical_device = device->get_physical_device();

        if (vkCreateBuffer(device->get_handle(), &buffer_info, nullptr, &m_handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer");
        }

        VkMemoryRequirements mem_requirements {};
        vkGetBufferMemoryRequirements(device->get_handle(), m_handle, &mem_requirements);

        m_memory->set_size(mem_requirements.size);
        m_memory->set_type(physical_device->get_suitable_memory_type(mem_requirements.memoryTypeBits, m_properties));
        m_memory->allocate();

        vkBindBufferMemory(device->get_handle(), m_handle, m_memory->get_handle(), 0);
    }

    void destroy() {
        if(!m_handle) return;

        auto device = m_memory->get_device();
        vkDestroyBuffer(device->get_handle(), m_handle, nullptr);
        m_handle = nullptr;
    }

    void copy(VK::CommandBuffer* command_buffer, VK::Buffer* destination, VkDeviceSize src_offset = 0, VkDeviceSize dst_offset = 0, VkDeviceSize size = UINT64_MAX) {

        if(src_offset >= m_size) throw std::invalid_argument("src_offset is greater than source buffer size");
        if(size > m_size - src_offset) size = m_size - src_offset;
        if(dst_offset + size > destination->m_size) throw std::invalid_argument("destination buffer overflow");

        VkBufferCopy copy_region {};
        copy_region.srcOffset = src_offset;
        copy_region.dstOffset = dst_offset;
        copy_region.size = size;

        vkCmdCopyBuffer(command_buffer->get_handle(), m_handle, destination->get_handle(), 1, &copy_region);
    }
};

}