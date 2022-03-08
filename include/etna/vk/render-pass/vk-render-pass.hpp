#pragma once

#include <vulkan/vulkan_core.h>
#include <etna/vk/resources/vk-device-resource.hpp>

namespace VK {

using UnownedRenderPass = UnownedDeviceResource<VkRenderPass>;

class RenderPass: public DeviceResource<VkRenderPass> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    RenderPass(RenderPass&& move) noexcept = default;
    RenderPass& operator=(RenderPass&& move_assign) = default;

    ~RenderPass() override { destroy(); }

    void destroy() final;
};

}