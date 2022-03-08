#pragma once

#include <span>
#include <vulkan/vulkan_core.h>
#include <etna/vk/device/vk-device.hpp>
#include <etna/vk/resources/vk-device-resource.hpp>

namespace VK {

using UnownedPipelineLayout = UnownedDeviceResource<VkPipelineLayout>;

class PipelineLayout: public DeviceResource<VkPipelineLayout> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    static PipelineLayout create(Device* device,
                   std::span<VkDescriptorSetLayout> descriptor_set_layouts,
                   std::span<VkPushConstantRange> push_constant_ranges);

    PipelineLayout(PipelineLayout&& move) noexcept = default;
    PipelineLayout& operator=(PipelineLayout&& move_assign) = default;

    ~PipelineLayout() override { destroy(); }

    void destroy() final;
};

}