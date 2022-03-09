
#include <etna/vk-wrappers/pipeline/vk-pipeline-input-vertex-state.hpp>

VK::PipelineVertexArrayBindingState VK::PipelineInputVertexState::create_binding(uint32_t binding, uint32_t stride) {
    return { this, binding, stride };
}

VkPipelineVertexInputStateCreateInfo VK::PipelineInputVertexState::compile() const {
    VkPipelineVertexInputStateCreateInfo result {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    result.vertexBindingDescriptionCount = m_descriptions.size();
    result.pVertexBindingDescriptions = m_descriptions.data();
    result.vertexAttributeDescriptionCount = m_attribute_descriptions.size();
    result.pVertexAttributeDescriptions = m_attribute_descriptions.data();

    return result;
}
