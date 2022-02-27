#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-physical-device.hpp"

namespace VK {

class Device {

    PhysicalDevice* m_physical_device;
    VkDevice m_handle;

public:

    Device(VkDevice device, PhysicalDevice* physical_device) {
        m_handle = device;
        m_physical_device = physical_device;
    }
    Device(const Device& copy) = delete;
    Device& operator=(const Device* copy_assign) = delete;

    ~Device() {
        destroy();
    }

    PhysicalDevice* get_physical_device() {
        return m_physical_device;
    }

    VkDevice get_handle() {
        return m_handle;
    }

    void wait_idle() {
        vkDeviceWaitIdle(m_handle);
    }

    void destroy() {
        if(!m_handle) return;
        vkDestroyDevice(m_handle, nullptr);
        m_handle = nullptr;
    }
};

}