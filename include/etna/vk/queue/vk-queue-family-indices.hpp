#pragma once

namespace VK {

struct DeviceQueueFamilies;

}

#include <cstdint>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <etna/vk/device/vk-physical-device.hpp>
#include <etna/vk/vk-surface.hpp>

namespace VK {

struct DeviceQueueFamilies {

    const VK::PhysicalDevice* m_physical_device;
    std::vector<VkQueueFamilyProperties> m_queue_families;

    explicit DeviceQueueFamilies(const PhysicalDevice* physical_device);

    int find_family(VkQueueFlagBits flags) const;
    int find_surface_present_family(const SurfaceBase& m_surface) const;
};

}