#pragma once

#include <span>
#include <etna/volk.hpp>
#include "vk-render-pass.hpp"
#include <etna/vk-wrappers/framebuffer.hpp>

namespace VK {

class RenderPassBeginInfo {
    VkRenderPassBeginInfo m_description {};
public:
    explicit RenderPassBeginInfo(const UnownedRenderPass& render_pass) {
        m_description.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        m_description.renderPass = render_pass.get_handle();
    }

    RenderPassBeginInfo& set_clear_values(std::span<VkClearValue> clear_values) {
        m_description.pClearValues = clear_values.data();
        m_description.clearValueCount = clear_values.size();
        return *this;
    }

    RenderPassBeginInfo& set_framebuffer(const VK::UnownedFramebuffer& framebuffer) {
        m_description.framebuffer = framebuffer.get_handle();
        return *this;
    }

    RenderPassBeginInfo& set_render_area(const VkRect2D& render_area) {
        m_description.renderArea = render_area;
        return *this;
    }

    VkRect2D& get_render_area() { return m_description.renderArea; }
    const VkRect2D& get_render_area() const { return m_description.renderArea; }

    VkRenderPassBeginInfo& get_description() { return m_description; }
    const VkRenderPassBeginInfo& get_description() const { return m_description; }
};

}