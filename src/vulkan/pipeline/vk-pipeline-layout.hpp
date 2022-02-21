#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "../vk-device.hpp"

namespace VK {

class PipelineLayout {
    VkPipelineLayout m_handle = nullptr;
    VK::Device* m_device;
public:
    PipelineLayout(Device* device,
                   std::vector<VkDescriptorSetLayout> descriptor_set_layouts,
                   std::vector<VkPushConstantRange> push_constant_ranges): m_device(device) {
        VkPipelineLayoutCreateInfo pipeline_layout_info {};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = descriptor_set_layouts.size();
        pipeline_layout_info.pSetLayouts = descriptor_set_layouts.data();
        pipeline_layout_info.pushConstantRangeCount = push_constant_ranges.size();
        pipeline_layout_info.pPushConstantRanges = push_constant_ranges.data();

        if(vkCreatePipelineLayout(device->get_handle(), &pipeline_layout_info, nullptr, &m_handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }

    ~PipelineLayout() {
        destroy();
    }

    void destroy() {
        if(m_handle) {
            vkDestroyPipelineLayout(m_device->get_handle(), m_handle, nullptr);
            m_handle = nullptr;
        }
    }

    VkPipelineLayout get_handle() { return m_handle; }
};

}