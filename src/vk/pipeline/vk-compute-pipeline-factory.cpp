
#include <etna/vk-wrappers/pipeline/vk-compute-pipeline-factory.hpp>

VK::Pipeline VK::ComputePipelineFactory::create(const VK::PipelineLayout &pipeline_layout) {
    VkComputePipelineCreateInfo pipeline_info {};
    pipeline_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipeline_info.layout = pipeline_layout.get_handle();

    // TODO:
    pipeline_info.stage = m_shader_stage.get_stage();
    pipeline_info.basePipelineHandle = nullptr;
    pipeline_info.basePipelineIndex = -1;

    auto device = pipeline_layout.get_device();

    VkPipeline pipeline {};
    if (vkCreateComputePipelines(device->get_handle(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create compute pipeline");
    }

    return { device, pipeline };
}
