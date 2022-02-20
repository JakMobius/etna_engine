#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "../vk-viewport.hpp"

namespace VK {

class PipelineViewportState {
    std::vector<VkViewport> m_viewports {};
    std::vector<VkRect2D> m_scissors {};

public:
    PipelineViewportState() {}

    void add_viewport(const Viewport& viewport) {
        m_viewports.push_back(viewport.get_viewport());
    }

    void add_viewport(const VkViewport& viewport) {
        m_viewports.push_back(viewport);
    }

    void add_scissor(const VkRect2D& scissor) {
        m_scissors.push_back(scissor);
    }

    const std::vector<VkViewport>& get_viewports() const { return m_viewports; }
    const std::vector<VkRect2D>& get_scissors() const { return m_scissors; }

    VkPipelineViewportStateCreateInfo compile() const {
        VkPipelineViewportStateCreateInfo viewport_state {};
        viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state.viewportCount = get_viewports().size();
        viewport_state.pViewports = get_viewports().data();
        viewport_state.scissorCount = get_scissors().size();
        viewport_state.pScissors = get_scissors().data();
        return viewport_state;
    }
};

}