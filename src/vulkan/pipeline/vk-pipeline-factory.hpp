#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>
#include "../vk-shader.hpp"

namespace VK {

class VertexArrayBinding {
    VkVertexInputBindingDescription m_descriptions;
    std::vector<VkVertexInputAttributeDescription> m_attribute_descriptions;
public:
    explicit VertexArrayBinding(uint32_t binding, uint32_t stride) {
        m_descriptions.binding = binding;
        m_descriptions.stride = stride;
        m_descriptions.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }

    void add_attribute_description(VkFormat format, uint32_t location, uint32_t offset) {
        m_attribute_descriptions.emplace_back();
        auto& descriptor = m_attribute_descriptions.back();

        descriptor.format = format;
        descriptor.binding = m_descriptions.binding;
        descriptor.location = location;
        descriptor.offset = offset;
    }

    const VkVertexInputBindingDescription* get_descriptions() {
        return &m_descriptions;
    }

    const std::vector<VkVertexInputAttributeDescription>& get_attribute_descriptions() {
        return m_attribute_descriptions;
    }
};

class PipelineShaderStages {
    std::vector<VkPipelineShaderStageCreateInfo> m_shader_stages {};
public:

    PipelineShaderStages() {}

    void add_shader(VK::Shader& shader, VkShaderStageFlagBits stage) {
        m_shader_stages.emplace_back();
        auto& shader_stage_info = m_shader_stages.back();

        shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stage_info.stage = stage;
        shader_stage_info.module = shader.get_handle();
        shader_stage_info.pName = "main";
    }

    const std::vector<VkPipelineShaderStageCreateInfo>& get_shader_stages() {
        return m_shader_stages;
    }
};

}