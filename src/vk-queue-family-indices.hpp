#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>
#include <vector>

struct VkQueueFamilyIndices {

    int m_graphics_family = -1;
    int m_present_family = -1;

    uint32_t m_queue_family_count = 0;
    std::vector<VkQueueFamilyProperties> queue_families;
    VkPhysicalDevice m_physical_device;

    VkQueueFamilyIndices(VkPhysicalDevice physical_device): m_physical_device(physical_device) {
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &m_queue_family_count, nullptr);
        queue_families.resize(m_queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &m_queue_family_count, queue_families.data());
    }

    void find_graphics_family() {
        for(int i = 0; i < m_queue_family_count; i++) {
            auto& family = queue_families[i];
            if(family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_graphics_family = i;
                break;
            }
        }
    }

    void find_present_family(VkSurfaceKHR m_surface) {
        for(int i = 0; i < m_queue_family_count; i++) {
            VkBool32 present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, i, m_surface, &present_support);
            if(present_support) {
                m_present_family = i;
                break;
            }
        }
    }

    bool is_complete() {
        return m_graphics_family >= 0;
    }
};