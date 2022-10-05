#pragma once

#include <etna/volk.hpp>
#include <etna/vk-wrappers/resources/vk-device-resource.hpp>

namespace VK {

using UnownedDescriptorPool = UnownedDeviceResource<VkDescriptorPool>;

class DescriptorPool: public DeviceResource<VkDescriptorPool> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    DescriptorPool(DescriptorPool&& move) noexcept = default;
    DescriptorPool& operator=(DescriptorPool&& move_assign) = default;

    ~DescriptorPool() override { destroy(); }

    void destroy() final;
};

}