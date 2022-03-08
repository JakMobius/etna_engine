//
// Created by Артем on 19.02.2022.
//

#include <etna/vk/pipeline/vk-pipeline-dynamic-state.hpp>

VkPipelineDynamicStateCreateInfo VK::PipelineDynamicState::compile() const {
    VkPipelineDynamicStateCreateInfo result = {};
    result.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    result.dynamicStateCount = m_dynamic_states.size();
    result.pDynamicStates = m_dynamic_states.data();
    return result;
}
