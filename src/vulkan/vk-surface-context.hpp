#pragma once

namespace VK {

class SurfaceContext;

}

#include <vulkan/vulkan_core.h>
#include <set>
#include "device/vk-device.hpp"
#include "command-buffer/vk-command-pool.hpp"
#include "command-buffer/vk-command-buffer.hpp"

namespace VK {

class SurfaceContext {

    PhysicalDevice* m_physical_device;
    VK::UnownedSurface m_surface;

    // Order of destruction of following fields matters:
    // Command pool should be deallocated before device.

    Device m_logical_device;
    CommandPool m_command_pool;

    int m_graphics_queue_family = -1;
    int m_present_queue_family = -1;

    VkQueue m_graphics_queue = nullptr;
    VkQueue m_present_queue = nullptr;

public:
    SurfaceContext(PhysicalDevice* device, const VK::UnownedSurface& surface);

    std::set<int> get_queue_families();

    void create_logical_device(const std::vector<const char*>& extensions, const std::vector<const char*>& validation_layers);

    void create_command_pool();

    VK::Device* get_device() { return &m_logical_device; }
    VK::CommandPool& get_command_pool() { return m_command_pool; }
    VK::UnownedSurface& get_surface() { return m_surface; }

    VkQueue get_device_graphics_queue() { return m_graphics_queue; }
    VkQueue get_device_present_queue() { return m_present_queue; }
};

}