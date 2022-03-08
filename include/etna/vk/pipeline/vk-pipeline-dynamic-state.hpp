#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace VK {

class PipelineDynamicState {
    std::vector<VkDynamicState> m_dynamic_states {};

public:
    PipelineDynamicState() = default;

    void add_dynamic_state(VkDynamicState state) {
        m_dynamic_states.push_back(state);
    }

    std::vector<VkDynamicState>& get_dynamic_states() {
        return m_dynamic_states;
    }

    VkPipelineDynamicStateCreateInfo compile() const;
};

}