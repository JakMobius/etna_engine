#pragma once

#include <span>
#include <vulkan/vulkan_core.h>
#include "../device/vk-device.hpp"
#include "../resources/vk-device-resource.hpp"

namespace VK {

using UnownedPipelineLayout = UnownedDeviceResource<VkPipelineLayout>;

class PipelineLayout: public DeviceResource<VkPipelineLayout> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    static PipelineLayout create(Device* device,
                   std::span<VkDescriptorSetLayout> descriptor_set_layouts,
                   std::span<VkPushConstantRange> push_constant_ranges) {
        VkPipelineLayoutCreateInfo pipeline_layout_info {};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = descriptor_set_layouts.size();
        pipeline_layout_info.pSetLayouts = descriptor_set_layouts.data();
        pipeline_layout_info.pushConstantRangeCount = push_constant_ranges.size();
        pipeline_layout_info.pPushConstantRanges = push_constant_ranges.data();

        VkPipelineLayout handle = nullptr;

        if(vkCreatePipelineLayout(device->get_handle(), &pipeline_layout_info, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout");
        }

        return { device, handle };
    }

    PipelineLayout(PipelineLayout&& move) noexcept = default;
    PipelineLayout& operator=(PipelineLayout&& move_assign) = default;

    ~PipelineLayout() override { destroy(); }

    void destroy() final {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyPipelineLayout(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }
};

}