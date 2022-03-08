#pragma once

namespace VK {
class PipelineVertexArrayBindingState;
}

#include <vulkan/vulkan_core.h>
#include <vector>
#include <etna/vk/vk-shader.hpp>
#include "vk-pipeline-input-vertex-state.hpp"

namespace VK {

class PipelineVertexArrayBindingState {

    uint32_t m_binding;
    VK::PipelineInputVertexState* m_input_vertex_state;
public:
    PipelineVertexArrayBindingState(VK::PipelineInputVertexState* input_vertex_state, uint32_t binding, uint32_t stride);

    void add_attribute(VkFormat format, uint32_t location, uint32_t offset);
};

}