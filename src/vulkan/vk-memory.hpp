#pragma once

namespace VK {
class Memory;
class MemoryReference;
}

#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include "vk-device.hpp"

namespace VK {

class MemoryReference {
    Memory* m_memory;
public:
    explicit MemoryReference(Memory* memory = nullptr);

    ~MemoryReference();

    Memory* get_memory() { return m_memory; }
};

class Memory {
    VkDeviceMemory m_memory = nullptr;
    Device* m_device = nullptr;
    mutable int m_references = 0;

public:
    explicit Memory(Device* device): m_device(device) {}
    Memory(): m_device(nullptr) {};

    VK::MemoryReference create_reference();

    void add_reference() {
        if(!m_memory) {
            throw std::runtime_error("attempt to create a reference to VK::Memory that is not allocated yet");
        }
        m_references++;
    };

    void remove_reference() {
        if(m_references == 0) {
            throw std::runtime_error("unbalanced calls to VK::Memory::add_reference and VK::Memory::remove_reference");
        }
        m_references--;
    };

    // As Memory class holds a Vulkan resource, we won't
    // allow one to copy/move it.
    Memory(const Memory& m_copy) = delete;
    Memory(Memory&& m_move) = delete;

    ~Memory() {
        free();
    }

    Device* get_device() { return m_device; }
    VkDeviceMemory get_handle() { return m_memory; }

    void set_device(Device* device) {
        m_device = device;
    }

    uint32_t get_suitable_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) {
        auto* mem_properties = m_device->get_memory_properties();

        for (uint32_t i = 0; i < mem_properties->memoryTypeCount; i++) {
            if ((type_filter & (1 << i)) && (mem_properties->memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
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

        if (vkAllocateMemory(m_device->get_handle(), &alloc_info, nullptr, &m_memory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }
    }

    void free() {
        if(!m_memory) return;
        if(m_references > 0) {
            throw std::runtime_error("attempt to free VK::Memory that is now being used");
        }

        vkFreeMemory(m_device->get_handle(), m_memory, nullptr);
        m_memory = nullptr;
    }
};

}