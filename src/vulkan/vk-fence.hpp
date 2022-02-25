#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-device-resource.hpp"

namespace VK {

using UnownedFence = UnownedDeviceResource<VkFence>;

class Fence: public DeviceResource<VkFence> {
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