
#include <etna/vk-wrappers/pipeline/vk-pipeline-rasterization-state.hpp>

VK::PipelineRasterizationState::PipelineRasterizationState() {
    m_description.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    m_description.polygonMode = VK_POLYGON_MODE_FILL;
    m_description.cullMode = VK_CULL_MODE_NONE;
    m_description.frontFace = VK_FRONT_FACE_CLOCKWISE;
    m_description.lineWidth = 1.0f;
    m_description.depthBiasEnable = VK_FALSE;
    m_description.depthBiasConstantFactor = 0.0f;
    m_description.depthBiasClamp = 0.0f;
    m_description.depthBiasSlopeFactor = 0.0f;
    m_description.depthClampEnable = VK_FALSE;
    m_description.rasterizerDiscardEnable = VK_FALSE;
}
