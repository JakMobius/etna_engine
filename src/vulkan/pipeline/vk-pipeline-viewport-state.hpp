#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "vk-viewport.hpp"

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

    const std::vector<VkViewport>& get_viewports() { return m_viewports; }
    const std::vector<VkRect2D>& get_scissors() { return m_scissors; }
};

}