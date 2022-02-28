#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class PipelineDepthStencilState {
    VkPipelineDepthStencilStateCreateInfo m_description {};

public:
    PipelineDepthStencilState();

    void set_depth_test_enable(VkBool32 depth_test_enable) {
        m_description.depthTestEnable = depth_test_enable;
    }
    void set_depth_write_enable(VkBool32 depth_write_enable) {
        m_description.depthWriteEnable = depth_write_enable;
    }
    void set_depth_compare_op(VkCompareOp depth_compare_op) {
        m_description.depthCompareOp = depth_compare_op;
    }
    void set_depth_bounds_test_enable(VkBool32 depth_bounds_test_enable) {
        m_description.depthBoundsTestEnable = depth_bounds_test_enable;
    }
    void set_min_depth_bounds(float min_depth_bounds) {
        m_description.minDepthBounds = min_depth_bounds;
    }
    void set_max_depth_bounds(float max_depth_bounds) {
        m_description.maxDepthBounds = max_depth_bounds;
    }
    void set_stencil_test_enable(VkBool32 stencil_test_enable) {
        m_description.stencilTestEnable = stencil_test_enable;
    }
    void set_front(VkStencilOpState front) {
        m_description.front = front;
    }
    void set_back(VkStencilOpState back) {
        m_description.back = back;
    }

    VkPipelineDepthStencilStateCreateInfo& get_description() { return m_description; }
};

}