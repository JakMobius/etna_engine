//
// Created by Артем on 09.02.2022.
//

#include "vk-queue-family-indices.hpp"

VK::QueueFamilyIndices::QueueFamilyIndices(const VK::PhysicalDevice* physical_device) : m_physical_device(physical_device) {
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device->get_handle(), &m_queue_family_count, nullptr);
    queue_families.resize(m_queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device->get_handle(), &m_queue_family_count, queue_families.data());
}

int VK::QueueFamilyIndices::find_surface_present_family(VkSurfaceKHR m_surface) const {
    for(int i = 0; i < m_queue_family_count; i++) {
        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device->get_handle(), i, m_surface, &present_support);
        if(present_support) return i;
    }
    return -1;
}

int VK::QueueFamilyIndices::find_family(VkQueueFlagBits flags) const {
    for(int i = 0; i < m_queue_family_count; i++) {
        auto &family = queue_families[i];
        if(family.queueFlags & flags) {
            return i;
        }
    }
    return -1;
}
