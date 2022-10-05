#pragma once

#include <etna/volk.hpp>
#include <etna/vk-wrappers/resources/vk-device-resource.hpp>
#include <etna/vk-wrappers/vk-memory.hpp>

namespace VK {

using UnownedDescriptorSetLayout = UnownedDeviceResource<VkDescriptorSetLayout>;

class DescriptorSetLayout: public DeviceResource<VkDescriptorSetLayout> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    DescriptorSetLayout(DescriptorSetLayout&& move) noexcept = default;
    DescriptorSetLayout& operator=(DescriptorSetLayout&& move_assign) = default;

    ~DescriptorSetLayout() override { destroy(); }

    void destroy() final;
};

}