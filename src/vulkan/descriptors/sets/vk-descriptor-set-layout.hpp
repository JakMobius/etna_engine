#pragma once

#include <vulkan/vulkan_core.h>
#include "../../vk-device-resource.hpp"
#include "../../vk-memory.hpp"

namespace VK {

using UnownedDescriptorSetLayout = UnownedDeviceResource<VkDescriptorSetLayout>;

class DescriptorSetLayout: public DeviceResource<VkDescriptorSetLayout> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    DescriptorSetLayout(DescriptorSetLayout&& move) noexcept = default;
    DescriptorSetLayout& operator=(DescriptorSetLayout&& move_assign) = default;

    ~DescriptorSetLayout() override { destroy(); }

    void destroy() {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyDescriptorSetLayout(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }
};

}