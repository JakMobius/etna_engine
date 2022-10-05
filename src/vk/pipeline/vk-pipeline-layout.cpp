
#include <etna/vk-wrappers/pipeline/vk-pipeline-layout.hpp>

VK::PipelineLayout
VK::PipelineLayout::create(VK::Device* device,
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

void VK::PipelineLayout::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkDestroyPipelineLayout(this->m_device->get_handle(), this->m_handle, nullptr);
    this->m_handle = nullptr;
}
