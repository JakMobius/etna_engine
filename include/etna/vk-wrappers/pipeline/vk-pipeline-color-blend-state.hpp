#pragma once

#include <vector>
#include <etna/volk.hpp>
#include "vk-pipeline-color-blend-attachment-state.hpp"

namespace VK {

class PipelineColorBlendState {
    std::vector<VkPipelineColorBlendAttachmentState> m_attachments {};

    bool m_logic_op_enable = false;
    VkLogicOp m_logic_op = VK_LOGIC_OP_MAX_ENUM;
    float m_blend_constants[4];

public:
    PipelineColorBlendState() {}

    PipelineColorBlendState& set_logic_op_enable(bool logic_op_enable) {
        m_logic_op_enable = logic_op_enable;
        return *this;
    }

    PipelineColorBlendState& set_logic_op(VkLogicOp logic_op) {
        m_logic_op = logic_op;
        return *this;
    }

    float(&get_blend_constants())[4] {
        return m_blend_constants;
    }

    PipelineColorBlendState& add_color_attachment(PipelineColorAttachmentState& attachment) {
        m_attachments.push_back(attachment.get_description());
        return *this;
    }

    VkPipelineColorBlendStateCreateInfo compile();
};

}