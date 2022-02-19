#pragma once

namespace VK {
class PipelineInputVertexState;
}

#include "vk-vertex-array-binding.hpp"

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

    VertexArrayBinding create_binding(uint32_t binding, uint32_t stride);
};

}