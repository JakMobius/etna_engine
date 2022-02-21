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
#include "vk-pipeline.hpp"

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

    VkPipeline create(Device* device, VkPipelineLayout pipeline_layout, VkRenderPass render_pass) {
        auto vk_vertex_input_info = input_vertex_state.compile();
        auto vk_viewport_state = viewport_state.compile();
        auto vk_color_blending = color_blend_state_create_info.compile();
        auto vk_dynamic_state = dynamic_states.compile();

        VkGraphicsPipelineCreateInfo pipeline_info {};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.stageCount = shader_stages.get_shader_stages().size();
        pipeline_info.pStages = shader_stages.get_shader_stages().data();

        pipeline_info.pVertexInputState = &vk_vertex_input_info;
        pipeline_info.pViewportState = &vk_viewport_state;
        pipeline_info.pColorBlendState = &vk_color_blending;
        pipeline_info.pDynamicState = &vk_dynamic_state;

        pipeline_info.pInputAssemblyState = &input_assembly.get_description();
        pipeline_info.pRasterizationState = &rasterization_state.get_description();
        pipeline_info.pMultisampleState = &multisampling_state.get_description();
        pipeline_info.pDepthStencilState = &depth_stencil_states.get_description();

        pipeline_info.layout = pipeline_layout;
        pipeline_info.renderPass = render_pass;

        // TODO:
        pipeline_info.subpass = 0;
        pipeline_info.basePipelineHandle = nullptr;
        pipeline_info.basePipelineIndex = -1;

        VkPipeline pipeline {};
        if (vkCreateGraphicsPipelines(device->get_handle(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline");
        }

        return pipeline;
    }
};

}