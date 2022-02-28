#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class PipelineColorAttachmentState {

    VkPipelineColorBlendAttachmentState m_description {};

public:
    PipelineColorAttachmentState();

    void set_color_write_mask(VkColorComponentFlags color_write_mask) {
        m_description.colorWriteMask = color_write_mask;
    }
    void set_blend_enable(VkBool32 blend_enable) {
        m_description.blendEnable = blend_enable;
    }
    void set_src_color_blend_factor(VkBlendFactor src_color_blend_factor) {
        m_description.srcColorBlendFactor = src_color_blend_factor;
    }
    void set_dst_color_blend_factor(VkBlendFactor dst_color_blend_factor) {
        m_description.dstColorBlendFactor = dst_color_blend_factor;
    }
    void set_color_blend_op(VkBlendOp color_blend_op) {
        m_description.colorBlendOp = color_blend_op;
    }
    void set_src_alpha_blend_factor(VkBlendFactor src_alpha_blend_factor) {
        m_description.srcAlphaBlendFactor = src_alpha_blend_factor;
    }
    void set_dst_alpha_blend_factor(VkBlendFactor dst_alpha_blend_factor) {
        m_description.dstAlphaBlendFactor = dst_alpha_blend_factor;
    }
    void set_alpha_blend_op(VkBlendOp alpha_blend_op) {
        m_description.alphaBlendOp = alpha_blend_op;
    }

    VkPipelineColorBlendAttachmentState& get_description() { return m_description; }
};

}