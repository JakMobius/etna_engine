#pragma once

namespace VK {

class SurfaceContext;

}

#include <vulkan/vulkan_core.h>
#include <set>
#include "vk-device.hpp"
#include "vk-command-pool.hpp"

namespace VK {

class SurfaceContext {

    PhysicalDevice* m_physical_device;
    VkSurfaceKHR m_surface;

    // Order of destruction of following fields matters:
    // Command pool should be deallocated before device.

    std::unique_ptr<VK::Device> m_logical_device;
    std::unique_ptr<VK::CommandPool> m_command_pool;

    int m_graphics_queue_index = -1;
    int m_present_queue_index = -1;

    VkQueue m_graphics_queue = nullptr;
    VkQueue m_present_queue = nullptr;

public:
    SurfaceContext(PhysicalDevice* device, VkSurfaceKHR surface);

    int get_graphics_queue_index() const { return m_graphics_queue_index; }
    int get_present_queue_index() const { return m_present_queue_index; }

    std::set<int> get_queue_families() {
        return {
            m_present_queue_index,
            m_graphics_queue_index
        };
    }

    void create_logical_device(const std::vector<const char*>& extensions, const std::vector<const char*>& validation_layers);

    void create_command_pool();

    VK::Device* get_device() {
        return m_logical_device.get();
    }

    VK::CommandPool* get_command_pool() {
        return m_command_pool.get();
    }

    VkQueue get_device_graphics_queue() { return m_graphics_queue; }
    VkQueue get_device_present_queue() { return m_present_queue; }
};

}