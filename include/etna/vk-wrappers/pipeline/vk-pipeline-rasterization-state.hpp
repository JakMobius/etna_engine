#pragma once

#include <etna/volk.hpp>

namespace VK {

class PipelineRasterizationState {
    VkPipelineRasterizationStateCreateInfo m_description {};
public:
    PipelineRasterizationState();

    PipelineRasterizationState& set_polygon_mode(VkPolygonMode polygon_mode) {
        m_description.polygonMode = polygon_mode;
        return *this;
    }

    PipelineRasterizationState& set_cull_mode(VkCullModeFlags cull_mode) {
        m_description.cullMode = cull_mode;
        return *this;
    }

    PipelineRasterizationState& set_front_face(VkFrontFace front_face) {
        m_description.frontFace = front_face;
        return *this;
    }

    PipelineRasterizationState& set_line_width(float line_width) {
        m_description.lineWidth = line_width;
        return *this;
    }

    PipelineRasterizationState& set_depth_bias_enable(VkBool32 depth_bias_enable) {
        m_description.depthBiasEnable = depth_bias_enable;
        return *this;
    }

    PipelineRasterizationState& set_depth_bias_constant_factor(float depth_bias_constant_factor) {
        m_description.depthBiasConstantFactor = depth_bias_constant_factor;
        return *this;
    }

    PipelineRasterizationState& set_depth_bias_clamp(float depth_bias_clamp) {
        m_description.depthBiasClamp = depth_bias_clamp;
        return *this;
    }

    PipelineRasterizationState& set_depth_bias_slope_factor(float depth_bias_slope_factor) {
        m_description.depthBiasSlopeFactor = depth_bias_slope_factor;
        return *this;
    }

    PipelineRasterizationState& set_depth_clamp_enable(VkBool32 depth_clamp_enable) {
        m_description.depthClampEnable = depth_clamp_enable;
        return *this;
    }

    PipelineRasterizationState& set_rasterizer_discard_enable(VkBool32 rasterizer_discard_enable) {
        m_description.rasterizerDiscardEnable = rasterizer_discard_enable;
        return *this;
    }


    VkPipelineRasterizationStateCreateInfo& get_description() { return m_description; }
};

}