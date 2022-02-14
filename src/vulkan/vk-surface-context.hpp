#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-device.hpp"

namespace VK {

class SurfaceContext {

    PhysicalDevice* m_device;
    VkSurfaceKHR m_surface;

    int m_graphics_queue_index = -1;
    int m_present_queue_index = -1;

public:
    SurfaceContext(PhysicalDevice* device, VkSurfaceKHR surface):
        m_device(device),
        m_surface(surface) {

        auto queue_family_indices = m_device->get_queue_family_indices();

        m_graphics_queue_index = queue_family_indices->find_family(VK_QUEUE_GRAPHICS_BIT);
        m_present_queue_index = queue_family_indices->find_surface_present_family(surface);
    }

    int get_graphics_queue_index() const { return m_graphics_queue_index; }
    int get_present_queue_index() const { return m_present_queue_index; }

};

}