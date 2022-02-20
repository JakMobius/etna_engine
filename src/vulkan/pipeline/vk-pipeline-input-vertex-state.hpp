#pragma once

namespace VK {
class PipelineInputVertexState;
}

#include "vk-pipeline-vertex-array-binding.hpp"

namespace VK {

class PipelineInputVertexState {
    std::vector<VkVertexInputBindingDescription> m_descriptions {};
    std::vector<VkVertexInputAttributeDescription> m_attribute_descriptions {};

public:
    PipelineInputVertexState() {

    }

    std::vector<VkVertexInputBindingDescription>& get_binding_descriptions() {
        return m_descriptions;
    }

    std::vector<VkVertexInputAttributeDescription>& get_attribute_descriptions() {
        return m_attribute_descriptions;
    }

    PipelineVertexArrayBindingState create_binding(uint32_t binding, uint32_t stride);

    VkPipelineVertexInputStateCreateInfo compile() const {
        VkPipelineVertexInputStateCreateInfo result {};
        result.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        result.vertexBindingDescriptionCount = m_descriptions.size();
        result.pVertexBindingDescriptions = m_descriptions.data();
        result.vertexAttributeDescriptionCount = m_attribute_descriptions.size();
        result.pVertexAttributeDescriptions = m_attribute_descriptions.data();

        return result;
    }
};

}