#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "vk-pipeline-color-blend-attachment-state.hpp"

namespace VK {

class PipelineColorBlendState {
    std::vector<VkPipelineColorBlendAttachmentState> m_attachments {};

    bool m_logic_op_enable = false;
    VkLogicOp m_logic_op = VK_LOGIC_OP_MAX_ENUM;
    float m_blend_constants[4];

public:
    PipelineColorBlendState() {}

    void set_logic_op_enable(bool logic_op_enable) {
        m_logic_op_enable = logic_op_enable;
    }

    void set_logic_op(VkLogicOp logic_op) {
        m_logic_op = logic_op;
    }

    float(&get_blend_constants())[4] {
        return m_blend_constants;
    }

    void add_color_attachment(PipelineColorAttachmentState& attachment) {
        m_attachments.push_back(attachment.get_description());
    }

    VkPipelineColorBlendStateCreateInfo compile() {
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
};

}