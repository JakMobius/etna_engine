#pragma once

#include <etna/volk.hpp>
#include <etna/vk-wrappers/device/vk-device.hpp>
#include <etna/vk-wrappers/resources/vk-device-resource.hpp>

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