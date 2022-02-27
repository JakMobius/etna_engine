#pragma once

namespace VK {

class PhysicalDevice;

}

#include <vector>
#include <vulkan/vulkan_core.h>
#include "../resources/vk-resource.hpp"

namespace VK {

class InstanceBase : public ResourceBase<VkInstance> {
public:
    using ResourceBase::ResourceBase;
    InstanceBase& operator=(InstanceBase&& move_assign) = default;

    std::vector<VkLayerProperties> get_validation_layers();

    std::vector<PhysicalDevice> get_physical_devices();

    std::vector<VkExtensionProperties> get_extensions();
};

using UnownedInstance = UnownedResource<VkInstance, InstanceBase>;

class Instance : public Resource<VkInstance, InstanceBase> {
public:
    using Resource::Resource;
    using Resource::operator=;

    Instance(Instance&& move) noexcept = default;
    Instance& operator=(Instance&& move_assign) = default;

    ~Instance() override { destroy(); }

    void destroy() final {
        if(!m_handle) return
        vkDestroyInstance(m_handle, nullptr);
        m_handle = nullptr;
    }
};

}