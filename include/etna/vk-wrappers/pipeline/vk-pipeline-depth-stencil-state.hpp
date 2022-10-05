#pragma once

#include <etna/volk.hpp>

namespace VK {

class PipelineDepthStencilState {
    VkPipelineDepthStencilStateCreateInfo m_description {};

public:
    PipelineDepthStencilState();


    PipelineDepthStencilState& set_depth_test_enable(VkBool32 depth_test_enable) {
        m_description.depthTestEnable = depth_test_enable;
        return *this;
    }

    PipelineDepthStencilState& set_depth_write_enable(VkBool32 depth_write_enable) {
        m_description.depthWriteEnable = depth_write_enable;
        return *this;
    }

    PipelineDepthStencilState& set_depth_compare_op(VkCompareOp depth_compare_op) {
        m_description.depthCompareOp = depth_compare_op;
        return *this;
    }

    PipelineDepthStencilState& set_depth_bounds_test_enable(VkBool32 depth_bounds_test_enable) {
        m_description.depthBoundsTestEnable = depth_bounds_test_enable;
        return *this;
    }

    PipelineDepthStencilState& set_min_depth_bounds(float min_depth_bounds) {
        m_description.minDepthBounds = min_depth_bounds;
        return *this;
    }

    PipelineDepthStencilState& set_max_depth_bounds(float max_depth_bounds) {
        m_description.maxDepthBounds = max_depth_bounds;
        return *this;
    }

    PipelineDepthStencilState& set_stencil_test_enable(VkBool32 stencil_test_enable) {
        m_description.stencilTestEnable = stencil_test_enable;
        return *this;
    }

    PipelineDepthStencilState& set_front(VkStencilOpState front) {
        m_description.front = front;
        return *this;
    }

    PipelineDepthStencilState& set_back(VkStencilOpState back) {
        m_description.back = back;
        return *this;
    }

    VkPipelineDepthStencilStateCreateInfo& get_description() { return m_description; }
};

}