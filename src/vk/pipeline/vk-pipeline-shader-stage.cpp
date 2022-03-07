//
// Created by Артем on 19.02.2022.
//

#include "vk-pipeline-shader-stage.hpp"

void VK::PipelineShaderStage::add_shader(VK::ShaderModule &shader, VkShaderStageFlagBits stage) {
    m_shader_stages.emplace_back();
    auto &shader_stage_info = m_shader_stages.back();

    shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_info.stage = stage;
    shader_stage_info.module = shader.get_handle();
    shader_stage_info.pName = "main";
}
