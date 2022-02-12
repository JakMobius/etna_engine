#pragma once

#include <vulkan/vulkan_core.h>
#include <stdexcept>

namespace VK {
class Memory {
    VkDeviceMemory m_memory = nullptr;
    VkDevice m_device = nullptr;

public:
    explicit Memory(VkDevice device): m_device(device) {}
    Memory(): m_device(nullptr) {};

    ~Memory() {
        free();
    }

    VkDeviceMemory get_handle() { return m_memory; }

    void set_device(VkDevice device) {
        m_device = device;
    }

    void allocate(uint64_t size, uint32_t memory_type) {
        if(!m_device) {
            throw std::runtime_error("cannot to allocate memory without specifying a device");
        }

        if(m_memory) free();

        VkMemoryAllocateInfo alloc_info {};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize = size;
        alloc_info.memoryTypeIndex = memory_type;

        if (vkAllocateMemory(m_device, &alloc_info, nullptr, &m_memory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }
    }

    void free() {
        if(!m_memory) return;

        vkFreeMemory(m_device, m_memory, nullptr);
        m_memory = nullptr;
    }
};
}