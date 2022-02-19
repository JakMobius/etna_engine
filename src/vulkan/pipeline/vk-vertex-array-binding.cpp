//
// Created by Артем on 19.02.2022.
//

#include "vk-vertex-array-binding.hpp"

VK::VertexArrayBinding::VertexArrayBinding(VK::PipelineInputVertexState* input_vertex_state, uint32_t binding,
                                           uint32_t stride) :
        m_input_vertex_state(input_vertex_state), m_binding(binding) {
    m_input_vertex_state->get_binding_descriptions().emplace_back();
    auto& description = m_input_vertex_state->get_binding_descriptions().back();

    description.binding = binding;
    description.stride = stride;
    description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
}

void VK::VertexArrayBinding::add_attribute(VkFormat format, uint32_t location, uint32_t offset) {
    m_input_vertex_state->get_attribute_descriptions().emplace_back();
    auto& descriptor = m_input_vertex_state->get_attribute_descriptions().back();

    descriptor.format = format;
    descriptor.binding = m_binding;
    descriptor.location = location;
    descriptor.offset = offset;
}
