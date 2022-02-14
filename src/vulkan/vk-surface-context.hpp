#pragma once

#include <vulkan/vulkan_core.h>
#include <set>
#include "vk-device.hpp"

namespace VK {

class SurfaceContext {

    PhysicalDevice* m_physical_device;
    VkSurfaceKHR m_surface;
    std::unique_ptr<VK::Device> m_logical_device;

    int m_graphics_queue_index = -1;
    int m_present_queue_index = -1;

    VkQueue m_graphics_queue = nullptr;
    VkQueue m_present_queue = nullptr;

public:
    SurfaceContext(PhysicalDevice* device, VkSurfaceKHR surface):
            m_physical_device(device),
            m_surface(surface) {

        auto queue_family_indices = m_physical_device->get_queue_family_indices();

        m_graphics_queue_index = queue_family_indices->find_family(VK_QUEUE_GRAPHICS_BIT);
        m_present_queue_index = queue_family_indices->find_surface_present_family(surface);
    }

    int get_graphics_queue_index() const { return m_graphics_queue_index; }
    int get_present_queue_index() const { return m_present_queue_index; }

    std::set<int> get_queue_families() {
        return {
            m_present_queue_index,
            m_graphics_queue_index
        };
    }

    void create_logical_device(const std::vector<const char*>& extensions, const std::vector<const char*> validation_layers) {
        std::vector<VkDeviceQueueCreateInfo> queues_to_create {};

        float queue_priority = 1.0f;

        for (int queue_family : get_queue_families()) {
            queues_to_create.emplace_back();
            VkDeviceQueueCreateInfo& queue_create_info = queues_to_create.back();

            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queue_family;
            queue_create_info.queueCount = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
        }

        VkPhysicalDeviceFeatures device_features {};
        device_features.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        create_info.pQueueCreateInfos = queues_to_create.data();
        create_info.queueCreateInfoCount = queues_to_create.size();
        create_info.pEnabledFeatures = &device_features;

        create_info.enabledExtensionCount = extensions.size();
        create_info.ppEnabledExtensionNames = extensions.data();

        create_info.enabledLayerCount = validation_layers.size();
        create_info.ppEnabledLayerNames = validation_layers.data();

        VkDevice device = nullptr;

        if (vkCreateDevice(m_physical_device->get_handle(), &create_info, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device");
        }

        m_logical_device = std::make_unique<VK::Device>(device, m_physical_device);

        vkGetDeviceQueue(m_logical_device->get_handle(), get_graphics_queue_index(), 0, &m_graphics_queue);
        vkGetDeviceQueue(m_logical_device->get_handle(), get_present_queue_index(), 0, &m_present_queue);
    }

    VK::Device* get_device() {
        return m_logical_device.get();
    }

    VkQueue get_device_graphics_queue() { return m_graphics_queue; }
    VkQueue get_device_present_queue() { return m_present_queue; }
};

}