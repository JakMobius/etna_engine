
#include <etna/vk-wrappers/pipeline/vk-pipeline-depth-stencil-state.hpp>

VK::PipelineDepthStencilState::PipelineDepthStencilState() {
    m_description.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

    m_description.depthTestEnable = VK_FALSE;
    m_description.depthWriteEnable = VK_FALSE;
    m_description.depthCompareOp = VK_COMPARE_OP_ALWAYS;
    m_description.depthBoundsTestEnable = VK_FALSE;
    m_description.minDepthBounds = 0.0f;
    m_description.maxDepthBounds = 1.0f;
    m_description.stencilTestEnable = VK_FALSE;
    m_description.front = {};
    m_description.back = {};
}
