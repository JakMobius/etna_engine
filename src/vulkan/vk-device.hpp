#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-physical-device.hpp"

namespace VK {

class Device {

    PhysicalDevice* m_physical_device;
    VkDevice m_handle;

public:

    // TODO: this is ugly, create device properly
    Device(VkDevice device = nullptr, PhysicalDevice* physical_device = nullptr) {
        m_handle = device;
        m_physical_device = physical_device;
    }

    PhysicalDevice* get_physical_device() {
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