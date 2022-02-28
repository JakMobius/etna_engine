#pragma once

#include <vulkan/vulkan_core.h>
#include "../device/vk-device.hpp"
#include "../resources/vk-device-resource.hpp"

namespace VK {

using UnownedSampler = UnownedDeviceResource<VkSampler>;

class Sampler: public DeviceResource<VkSampler> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    Sampler(Sampler&& move) noexcept = default;
    Sampler& operator=(Sampler&& move_assign) = default;

    ~Sampler() override { destroy(); }

    void destroy() final;
};

}