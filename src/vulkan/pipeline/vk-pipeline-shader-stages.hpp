#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "../vk-shader.hpp"

namespace VK {

class PipelineShaderStages {
    std::vector <VkPipelineShaderStageCreateInfo> m_shader_stages {};
public:

    PipelineShaderStages() {}

    void add_shader(VK::Shader &shader, VkShaderStageFlagBits stage) {
        m_shader_stages.emplace_back();
        auto &shader_stage_info = m_shader_stages.back();

        shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stage_info.stage = stage;
        shader_stage_info.module = shader.get_handle();
        shader_stage_info.pName = "main";
    }

    const std::vector <VkPipelineShaderStageCreateInfo> &get_shader_stages() {
        return m_shader_stages;
    }
};

}