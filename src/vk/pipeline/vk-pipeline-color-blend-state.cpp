
#include <etna/vk-wrappers/pipeline/vk-pipeline-color-blend-state.hpp>

VkPipelineColorBlendStateCreateInfo VK::PipelineColorBlendState::compile() {
    VkPipelineColorBlendStateCreateInfo result {};

    result.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    result.logicOpEnable = m_logic_op_enable ? VK_TRUE : VK_FALSE;
    result.logicOp = m_logic_op;
    result.attachmentCount = m_attachments.size();
    result.pAttachments = m_attachments.data();
    for(int i = 0; i < 4; i++) {
        result.blendConstants[i] = m_blend_constants[i];
    }

    return result;
}
