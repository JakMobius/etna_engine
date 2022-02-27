#pragma once

#include <vulkan/vulkan_core.h>
#include "../resources/vk-device-resource.hpp"

namespace VK {

using UnownedRenderPass = UnownedDeviceResource<VkRenderPass>;

class RenderPass: public DeviceResource<VkRenderPass> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    RenderPass(RenderPass&& move) noexcept = default;
    RenderPass& operator=(RenderPass&& move_assign) = default;

    ~RenderPass() override { destroy(); }

    void destroy() {
        if(!m_handle) return;
        vkDestroyRenderPass(m_device->get_handle(), m_handle, nullptr);
        m_handle = nullptr;
    }
};

}