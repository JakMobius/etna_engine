
#include <etna/vk-wrappers/pipeline/vk-pipeline-color-blend-attachment-state.hpp>

VK::PipelineColorAttachmentState::PipelineColorAttachmentState() {
    m_description.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT |
            VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;

    m_description.blendEnable = VK_FALSE;
    m_description.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    m_description.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    m_description.colorBlendOp = VK_BLEND_OP_ADD;
    m_description.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    m_description.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    m_description.alphaBlendOp = VK_BLEND_OP_ADD;
}
