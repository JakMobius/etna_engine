#pragma once

namespace VK {
class Memory;
}

#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include "device/vk-device.hpp"

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
    Memory& operator=(Memory&& move_assign) noexcept;
    Memory(const Memory& copy) = delete;
    Memory& operator=(const Memory& copy_assign) = delete;

    ~Memory() { free(); }

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

    void allocate();

    void* map();

    void unmap();

    void free();

    void flush();
    void flush(uint32_t offset, uint32_t size);

    void set_data(void* data, size_t size, size_t offset = 0);
};

}