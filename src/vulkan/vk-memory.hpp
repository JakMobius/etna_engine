#pragma once

namespace VK {
class Memory;
}

#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include "vk-device.hpp"

namespace VK {

class Memory {
    VkDeviceMemory m_handle = nullptr;
    Device* m_device = nullptr;
    uint32_t m_size = 0;
    uint32_t m_memory_type = 0;

public:
    explicit Memory(Device* device): m_device(device) {}
    Memory(): m_device(nullptr) {};
    Memory(Memory&& move) noexcept: m_handle(move.m_handle), m_size(move.m_size), m_device(move.m_device), m_memory_type(move.m_memory_type) {
        move.m_handle = nullptr;
    }
    Memory& operator=(Memory&& move_assign) noexcept {
        if(this == &move_assign) return *this;
        free();
        m_device = move_assign.m_device;
        m_handle = move_assign.m_handle;
        m_size = move_assign.m_size;
        m_memory_type = move_assign.m_memory_type;
        move_assign.m_handle = nullptr;
        return *this;
    }
    Memory(const Memory& copy) = delete;
    Memory& operator=(const Memory& copy_assign) = delete;

    ~Memory() {
        free();
    }

    Device* get_device() { return m_device; }
    VkDeviceMemory get_handle() { return m_handle; }

    void set_device(Device* device) {
        m_device = device;
    }

    void set_size(uint64_t size) {
        m_size = size;
    }

    void set_type(uint32_t memory_type) {
        m_memory_type = memory_type;
    }

    void allocate() {
        if(!m_device) {
            throw std::runtime_error("cannot to allocate memory without specifying a device");
        }

        if(m_size == 0) {
            throw std::runtime_error("attempt to allocate zero-length VK::Memory");
        }

        if(m_handle) free();

        VkMemoryAllocateInfo alloc_info {};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize = m_size;
        alloc_info.memoryTypeIndex = m_memory_type;

        if (vkAllocateMemory(m_device->get_handle(), &alloc_info, nullptr, &m_handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }
    }

    void* map() {
        void* mapped_ptr = nullptr;
        vkMapMemory(m_device->get_handle(), m_handle, 0, m_size, 0, &mapped_ptr);
        return mapped_ptr;
    }

    void unmap() {
        vkUnmapMemory(m_device->get_handle(), m_handle);
    }

    void free() {
        if(!m_handle) return;

        vkFreeMemory(m_device->get_handle(), m_handle, nullptr);
        m_handle = nullptr;
    }

    void flush();
    void flush(uint32_t offset, uint32_t size);

    void set_data(void* data, size_t size, size_t offset = 0) {
        void* mapped_ptr = map();
        memcpy(mapped_ptr, data, size);
        flush(offset, size);
        unmap();
    }
};

}