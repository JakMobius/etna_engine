
#include <etna/vk-wrappers/pipeline/vk-pipeline-shader-stages.hpp>

void VK::PipelineShaderStages::add_shader(VK::ShaderModule &shader, VkShaderStageFlagBits stage, const char* entry_point) {
    m_shader_stages.emplace_back();
    auto &shader_stage_info = m_shader_stages.back();

    shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_info.stage = stage;
    shader_stage_info.module = shader.get_handle();
    shader_stage_info.pName = entry_point;
}
