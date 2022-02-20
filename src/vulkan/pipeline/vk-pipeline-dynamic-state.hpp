#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace VK {

class PipelineDynamicState {
    std::vector<VkDynamicState> m_dynamic_states {};

public:
    PipelineDynamicState() {

    }

    void add_dynamic_state(VkDynamicState state) {
        m_dynamic_states.push_back(state);
    }

    std::vector<VkDynamicState>& get_dynamic_states() {
        return m_dynamic_states;
    }

    VkPipelineDynamicStateCreateInfo compile() const {
        VkPipelineDynamicStateCreateInfo result = {};
        result.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        result.dynamicStateCount = m_dynamic_states.size();
        result.pDynamicStates = m_dynamic_states.data();
        return result;
    }
};

}