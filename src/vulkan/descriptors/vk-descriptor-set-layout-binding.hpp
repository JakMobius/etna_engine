#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class DescriptorSetLayoutBinding {
    VkDescriptorSetLayoutBinding m_description {};

public:
    explicit DescriptorSetLayoutBinding(VkDescriptorType type);

    void set_descriptor_type(VkDescriptorType descriptor_type) { m_description.descriptorType = descriptor_type; };
    void set_descriptor_count(uint32_t descriptor_count) { m_description.descriptorCount = descriptor_count; };
    void set_stage_flags(VkShaderStageFlags stage_flags) { m_description.stageFlags = stage_flags; };
    void set_immutable_samplers(const VkSampler* p_immutable_samplers) { m_description.pImmutableSamplers = p_immutable_samplers; };

    VkDescriptorSetLayoutBinding& get_description() { return m_description; }
};

}
