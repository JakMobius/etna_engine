#pragma once

#include <vector>
#include <etna/volk.hpp>
#include <etna/vk-wrappers/vk-viewport.hpp>

namespace VK {

class PipelineViewportState {
    std::vector<VkViewport> m_viewports {};
    std::vector<VkRect2D> m_scissors {};

public:
    PipelineViewportState() = default;

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

    VkPipelineViewportStateCreateInfo compile() const;
};

}