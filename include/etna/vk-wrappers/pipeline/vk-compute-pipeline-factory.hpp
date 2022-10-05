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
#include "vk-pipeline-shader-stage.hpp"

namespace VK {

class ComputePipelineFactory {
public:
    PipelineShaderStage m_shader_stage;
    Pipeline create(const PipelineLayout &pipeline_layout);
};

}