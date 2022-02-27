#pragma once

namespace VK {

struct QueueFamilyIndices;

}

#include <cstdint>
#include <vulkan/vulkan_core.h>
#include <vector>
#include "vk-physical-device.hpp"
#include "vk-surface.hpp"

namespace VK {

struct QueueFamilyIndices {

    uint32_t m_queue_family_count = 0;
    std::vector<VkQueueFamilyProperties> queue_families;
    const PhysicalDevice* m_physical_device;

    explicit QueueFamilyIndices(const PhysicalDevice* physical_device);

    int find_family(VkQueueFlagBits flags) const;
    int find_surface_present_family(const SurfaceBase& m_surface) const;
};

}