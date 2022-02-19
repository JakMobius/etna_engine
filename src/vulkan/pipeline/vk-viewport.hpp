#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class Viewport {
    VkViewport m_viewport {};
public:

    explicit Viewport(VkExtent2D size): Viewport((float)size.width, (float)size.height) {}

    explicit Viewport(float width, float height) {
        m_viewport.x = 0.0f;
        m_viewport.y = 0.0f;
        m_viewport.width = width;
        m_viewport.height = height;
        m_viewport.minDepth = 0.0f;
        m_viewport.maxDepth = 1.0f;
    }

    void set_position(float x, float y) {
        m_viewport.x = x;
        m_viewport.y = y;
    }

    void set_size(float width, float height) {
        m_viewport.width = width;
        m_viewport.height = height;
    }

    void set_depth_limits(float min, float max) {
        m_viewport.minDepth = min;
        m_viewport.maxDepth = max;
    }

    const VkViewport& get_viewport() const { return m_viewport; }
};

}