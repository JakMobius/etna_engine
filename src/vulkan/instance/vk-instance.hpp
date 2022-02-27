#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "../vk-physical-device.hpp"
#include "../resources/vk-resource.hpp"

namespace VK {

class InstanceBase : public ResourceBase<VkInstance> {
public:
    using ResourceBase::ResourceBase;
    InstanceBase& operator=(InstanceBase&& move_assign) = default;

    std::vector<VkLayerProperties> get_validation_layers() {
        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        std::vector<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
        return available_layers;
    }

    std::vector<PhysicalDevice> get_physical_devices() {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(m_handle, &device_count, nullptr);
        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(m_handle, &device_count, devices.data());

        std::vector<PhysicalDevice> physical_devices {devices.begin(), devices.end()};

        return physical_devices;
    }

    std::vector<VkExtensionProperties> get_extensions() {
        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
        std::vector<VkExtensionProperties> extensions(extension_count);
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
        return extensions;
    }
};

using UnownedInstance = UnownedResource<VkInstance, InstanceBase>;

class Instance : public Resource<VkInstance, InstanceBase> {
public:
    using Resource::Resource;
    using Resource::operator=;

    Instance(Instance&& move) noexcept = default;
    Instance& operator=(Instance&& move_assign) = default;

    ~Instance() override { destroy(); }

    void destroy() {
        if(!m_handle) return
        vkDestroyInstance(m_handle, nullptr);
        m_handle = nullptr;
    }
};

}