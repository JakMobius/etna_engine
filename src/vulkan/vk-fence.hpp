#pragma once

#include <vulkan/vulkan_core.h>
#include "resources/vk-device-resource.hpp"

namespace VK {

class FenceBase: public DeviceResourceBase<VkFence> {
public:
    using DeviceResourceBase::DeviceResourceBase;
    FenceBase& operator=(FenceBase&& move_assign) = default;

    void wait_one(uint64_t timeout = UINT64_MAX) const {
        vkWaitForFences(m_device->get_handle(), 1, &m_handle, VK_TRUE, timeout);
    }

    void reset_one() {
        vkResetFences(m_device->get_handle(), 1, &m_handle);
    }

    ~FenceBase() override = default;
};

using UnownedFence = UnownedDeviceResource<VkFence, FenceBase>;

class Fence: public DeviceResource<VkFence, FenceBase> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    Fence(Fence&& move) noexcept = default;
    Fence& operator=(Fence&& move_assign) = default;

    static Fence create(VK::Device* device, VkFenceCreateFlags flags = 0) {
        VkFenceCreateInfo fence_info {};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = flags;

        VkFence fence = nullptr;
        if(vkCreateFence(device->get_handle(), &fence_info, nullptr, &fence)) {
            throw std::runtime_error("failed to create a fence");
        }

        return {device, fence};
    }

    ~Fence() override { destroy(); }

    void destroy() {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyFence(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }
};

}