
#include <etna/vk-wrappers/pipeline/vk-pipeline-shader-stage.hpp>

void VK::PipelineShaderStage::set_shader(const VK::UnownedShaderModule& shader, VkShaderStageFlagBits stage, const char* entry_point) {
    m_shader_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_shader_stage.stage = stage;
    m_shader_stage.module = shader.get_handle();
    m_shader_stage.pName = entry_point;
}
