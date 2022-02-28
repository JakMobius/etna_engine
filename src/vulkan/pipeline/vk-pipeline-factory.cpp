//
// Created by Артем on 21.02.2022.
//

#include "vk-pipeline-factory.hpp"

VK::Pipeline VK::PipelineFactory::create(const VK::PipelineLayout &pipeline_layout, const VK::RenderPass &render_pass) {
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

    pipeline_info.layout = pipeline_layout.get_handle();
    pipeline_info.renderPass = render_pass.get_handle();

    // TODO:
    pipeline_info.subpass = 0;
    pipeline_info.basePipelineHandle = nullptr;
    pipeline_info.basePipelineIndex = -1;

    auto device = pipeline_layout.get_device();

    VkPipeline pipeline {};
    if (vkCreateGraphicsPipelines(device->get_handle(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline");
    }

    return { device, pipeline };
}
