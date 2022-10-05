#pragma once

#include <etna/volk.hpp>
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
    DeviceFactory();

    DeviceFactory& set_flags(VkDeviceCreateFlags flags) {
        m_description.flags = flags;
        return *this;
    }

    Device create(PhysicalDevice* physical_device);

    DeviceFactory& add_queue(VkDeviceQueueCreateFlags flags, uint32_t family, std::span<float> priorities);

    std::vector<VkDeviceQueueCreateInfo>& get_queues_to_create() { return m_queues_to_create; }
    std::vector<const char*>& get_enabled_extensions() { return m_enabled_extensions; }
    std::vector<const char*>& get_validation_layers() { return m_validation_layers; }
    VkPhysicalDeviceFeatures& get_device_features() { return m_features; }
};

}