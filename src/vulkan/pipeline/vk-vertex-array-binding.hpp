#pragma once

namespace VK {
class VertexArrayBindingState;
}

#include <vulkan/vulkan_core.h>
#include <vector>
#include "../vk-shader.hpp"
#include "vk-pipeline-input-vertex-state.hpp"

namespace VK {

class VertexArrayBindingState {

    uint32_t m_binding;
    VK::PipelineInputVertexState* m_input_vertex_state;
public:
    VertexArrayBindingState(VK::PipelineInputVertexState* input_vertex_state, uint32_t binding, uint32_t stride);

    void add_attribute(VkFormat format, uint32_t location, uint32_t offset);
};

}