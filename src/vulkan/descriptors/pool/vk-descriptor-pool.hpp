#pragma once

#include <vulkan/vulkan_core.h>
#include "../../resources/vk-device-resource.hpp"

namespace VK {

using UnownedDescriptorPool = UnownedDeviceResource<VkDescriptorPool>;

class DescriptorPool: public DeviceResource<VkDescriptorPool> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    DescriptorPool(DescriptorPool&& move) noexcept = default;
    DescriptorPool& operator=(DescriptorPool&& move_assign) = default;

    ~DescriptorPool() override { destroy(); }

    void destroy() {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyDescriptorPool(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }
};

}