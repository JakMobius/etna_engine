#pragma once

#include "vk-pipeline-shader-stage.hpp"
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
#include "../render-pass/vk-render-pass.hpp"

namespace VK {

class PipelineFactory {
public:
    PipelineShaderStage shader_stages {};
    PipelineInputVertexState input_vertex_state {};
    PipelineViewportState viewport_state {};
    PipelineInputAssemblyStates input_assembly {};
    PipelineRasterizationState rasterization_state {};
    PipelineMultisamplingState multisampling_state {};
    PipelineDynamicState dynamic_states {};
    PipelineColorBlendState color_blend_state_create_info {};
    PipelineDepthStencilState depth_stencil_states {};

    Pipeline create(const PipelineLayout& pipeline_layout, const RenderPass& render_pass);
};

}