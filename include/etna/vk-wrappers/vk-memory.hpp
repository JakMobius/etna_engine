#pragma once

namespace VK {
class Memory;
}

#include <etna/volk.hpp>
#include <stdexcept>
#include <etna/vk-wrappers/device/vk-device.hpp>
#include <cstring>

namespace VK {

class Memory {
    VkDeviceMemory m_handle = nullptr;
    Device* m_device = nullptr;
    uint32_t m_size = 0;
    uint32_t m_memory_type = 0;

public:
    explicit Memory(Device* device): m_device(device) {}
    Memory(): m_device(nullptr) {};
    Memory(Memory&& move) noexcept;
    Memory& operator=(Memory&& move_assign) noexcept;
    Memory(const Memory& copy) = delete;
    Memory& operator=(const Memory& copy_assign) = delete;

    ~Memory() { free(); }

    Device* get_device() { return m_device; }
    VkDeviceMemory get_handle() { return m_handle; }
    uint32_t get_size() { return m_size; }
    uint32_t get_memory_type() { return m_memory_type; }

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

    bool is_null() { return m_handle == nullptr; }
    void set_data(const void* data, size_t size, size_t offset = 0);
    void get_data(void* data, size_t size, size_t offset = 0);
};

}