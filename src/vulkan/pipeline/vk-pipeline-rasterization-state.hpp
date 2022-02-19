#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class PipelineRasterizationState {
    VkPipelineRasterizationStateCreateInfo m_description {};
public:
    PipelineRasterizationState() {
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

    void set_polygon_mode(VkPolygonMode polygon_mode) {
        m_description.polygonMode = polygon_mode;
    }
    void set_cull_mode(VkCullModeFlags cull_mode) {
        m_description.cullMode = cull_mode;
    }
    void set_front_face(VkFrontFace front_face) {
        m_description.frontFace = front_face;
    }
    void set_line_width(float line_width) {
        m_description.lineWidth = line_width;
    }
    void set_depth_bias_enable(VkBool32 depth_bias_enable) {
        m_description.depthBiasEnable = depth_bias_enable;
    }
    void set_depth_bias_constant_factor(float depth_bias_constant_factor) {
        m_description.depthBiasConstantFactor = depth_bias_constant_factor;
    }
    void set_depth_bias_clamp(float depth_bias_clamp) {
        m_description.depthBiasClamp = depth_bias_clamp;
    }
    void set_depth_bias_slope_factor(float depth_bias_slope_factor) {
        m_description.depthBiasSlopeFactor = depth_bias_slope_factor;
    }
    void set_depth_clamp_enable(VkBool32 depth_clamp_enable) {
        m_description.depthClampEnable = depth_clamp_enable;
    }
    void set_rasterizer_discard_enable(VkBool32 rasterizer_discard_enable) {
        m_description.rasterizerDiscardEnable = rasterizer_discard_enable;
    }

    VkPipelineRasterizationStateCreateInfo& get_description() { return m_description; }
};

}