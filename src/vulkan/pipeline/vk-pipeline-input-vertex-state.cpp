//
// Created by Артем on 19.02.2022.
//

#include "vk-pipeline-input-vertex-state.hpp"

VK::VertexArrayBindingState VK::PipelineInputVertexState::create_binding(uint32_t binding, uint32_t stride) {
    return { this, binding, stride };
}
