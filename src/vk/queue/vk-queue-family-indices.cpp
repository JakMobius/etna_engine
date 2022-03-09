
#include <etna/vk-wrappers/queue/vk-queue-family-indices.hpp>

VK::DeviceQueueFamilies::DeviceQueueFamilies(const VK::PhysicalDevice* physical_device) : m_physical_device(physical_device) {
    uint32_t family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device->get_handle(), &family_count, nullptr);
    m_queue_families.resize(family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device->get_handle(), &family_count, m_queue_families.data());
}

int VK::DeviceQueueFamilies::find_surface_present_family(const VK::SurfaceBase& m_surface) const {
    for(int i = 0; i < m_queue_families.size(); i++) {
        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device->get_handle(), i, m_surface.get_handle(), &present_support);
        if(present_support) return i;
    }
    return -1;
}

int VK::DeviceQueueFamilies::find_family(VkQueueFlagBits flags) const {
    for(int i = 0; i < m_queue_families.size(); i++) {
        auto &family = m_queue_families[i];
        if(family.queueFlags & flags) {
            return i;
        }
    }
    return -1;
}
