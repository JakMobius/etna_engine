#pragma once

#include <vulkan/vulkan_core.h>
#include "../vk-physical-device.hpp"

namespace VK {

class Device {

    PhysicalDevice* m_physical_device = nullptr;
    VkDevice m_handle = nullptr;

public:
    Device() = default;
    Device(VkDevice device, PhysicalDevice* physical_device) {
        m_handle = device;
        m_physical_device = physical_device;
    }
    Device(const Device& copy) = delete;
    Device& operator=(const Device& copy_assign) = delete;

    Device(Device&& move) noexcept { *this = std::move(move); };
    Device& operator=(Device&& move_assign)  noexcept {
        if(this == &move_assign) return *this;
        destroy();
        m_physical_device = move_assign.m_physical_device;
        m_handle = move_assign.m_handle;
        move_assign.m_handle = nullptr;
        return *this;
    };

    ~Device() { destroy(); }

    PhysicalDevice* get_physical_device() {
        return m_physical_device;
    }

    void wait_idle() {
        vkDeviceWaitIdle(m_handle);
    }

    VkQueue get_queue(uint32_t queue_family, uint32_t index) {
        VkQueue result = nullptr;
        vkGetDeviceQueue(m_handle, queue_family, index, &result);
        return result;
    }

    void destroy() {
        if(!m_handle) return;
        vkDestroyDevice(m_handle, nullptr);
        m_handle = nullptr;
    }

    bool is_null() { return !m_handle; }
    explicit operator bool() { return !is_null(); }

    VkDevice get_handle() { return m_handle; }
};

}