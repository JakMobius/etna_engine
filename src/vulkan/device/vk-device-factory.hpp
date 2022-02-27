#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>
#include <span>
#include "vk-device.hpp"

namespace VK {

class DeviceFactory {
    VkDeviceCreateInfo m_description {};
    VkPhysicalDeviceFeatures m_features {};
    std::vector<VkDeviceQueueCreateInfo> m_queues_to_create {};
    std::vector<const char*> m_enabled_extensions {};
    std::vector<const char*> m_validation_layers {};
public:
    DeviceFactory() {
        m_description.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        m_description.pEnabledFeatures = &m_features;
    }

    void set_flags(VkDeviceCreateFlags flags) {
        m_description.flags = flags;
    }

    Device create(PhysicalDevice* physical_device) {
        m_description.pQueueCreateInfos = m_queues_to_create.data();
        m_description.queueCreateInfoCount = m_queues_to_create.size();

        m_description.enabledExtensionCount = m_enabled_extensions.size();
        m_description.ppEnabledExtensionNames = m_enabled_extensions.data();

        m_description.enabledLayerCount = m_validation_layers.size();
        m_description.ppEnabledLayerNames = m_validation_layers.data();

        VkDevice device = nullptr;

        if (vkCreateDevice(physical_device->get_handle(), &m_description, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device");
        }

        return { device, physical_device };
    }

    void add_queue(VkDeviceQueueCreateFlags flags, uint32_t family, std::span<float> priorities) {
        m_queues_to_create.emplace_back();
        VkDeviceQueueCreateInfo& queue_create_info = m_queues_to_create.back();
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = family;
        queue_create_info.flags = flags;
        queue_create_info.queueCount = priorities.size();
        queue_create_info.pQueuePriorities = priorities.data();
    }

    std::vector<VkDeviceQueueCreateInfo>& get_queues_to_create() { return m_queues_to_create; }
    std::vector<const char*>& get_enabled_extensions() { return m_enabled_extensions; }
    std::vector<const char*>& get_validation_layers() { return m_validation_layers; }
    VkPhysicalDeviceFeatures& get_device_features() { return m_features; }
};

}