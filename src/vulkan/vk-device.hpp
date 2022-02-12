#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class Device {

    VkPhysicalDeviceMemoryProperties m_mem_properties {};
    VkPhysicalDevice m_physical_device;
    VkDevice m_handle;

public:

    // TODO: this is ugly, create device properly
    Device(VkDevice device = nullptr, VkPhysicalDevice physical_device = nullptr) {
        m_handle = device;
        m_physical_device = physical_device;
        if(m_physical_device != nullptr) {
            vkGetPhysicalDeviceMemoryProperties(m_physical_device, &m_mem_properties);
        }
    }

    const VkPhysicalDeviceMemoryProperties* get_memory_properties() {
        return &m_mem_properties;
    }

    VkPhysicalDevice get_physical_device() {
        return m_physical_device;
    }

    VkDevice get_handle() {
        return m_handle;
    }

    void destroy() {
        if(m_handle) {
            vkDestroyDevice(m_handle, nullptr);
            m_handle = nullptr;
        }
    }
};

}