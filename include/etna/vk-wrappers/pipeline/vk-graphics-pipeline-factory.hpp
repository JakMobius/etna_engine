#pragma once

#include "vk-pipeline-shader-stages.hpp"
#include "vk-pipeline-viewport-state.hpp"
#include "vk-pipeline-input-assembly-state.hpp"
#include "vk-pipeline-rasterization-state.hpp"
#include "vk-pipeline-multisampling-state.hpp"
#include "vk-pipeline-dynamic-state.hpp"
#include "vk-pipeline-color-blend-state.hpp"
#include "vk-pipeline-depth-stencil-state.hpp"
#include "vk-pipeline-input-vertex-state.hpp"
#include "vk-pipeline-layout.hpp"
#include "vk-pipeline.hpp"
#include <etna/vk-wrappers/render-pass/vk-render-pass.hpp>

namespace VK {

class PipelineFactory {
public:
    PipelineShaderStages m_shader_stages {};
    PipelineInputVertexState m_input_vertex_state {};
    PipelineViewportState m_viewport_state {};
    PipelineInputAssemblyStates m_input_assembly {};
    PipelineRasterizationState m_rasterization_state {};
    PipelineMultisamplingState m_multisampling_state {};
    PipelineDynamicState m_dynamic_states {};
    PipelineColorBlendState m_color_blend_state_create_info {};
    PipelineDepthStencilState m_depth_stencil_states {};

    Pipeline create(const PipelineLayout& pipeline_layout, const RenderPass& render_pass);
};

}