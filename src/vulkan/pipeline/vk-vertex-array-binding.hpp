#pragma once

namespace VK {
class VertexArrayBinding;
}

#include <vulkan/vulkan_core.h>
#include <vector>
#include "../vk-shader.hpp"
#include "vk-pipeline-input-vertex-state.hpp"

namespace VK {

class VertexArrayBinding {

    uint32_t m_binding;
    VK::PipelineInputVertexState* m_input_vertex_state;
public:
    VertexArrayBinding(VK::PipelineInputVertexState* input_vertex_state, uint32_t binding, uint32_t stride);

    void add_attribute(VkFormat format, uint32_t location, uint32_t offset);
};

}