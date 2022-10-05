#pragma once

#include <etna/volk.hpp>

namespace VK {

class Viewport {
    VkViewport m_viewport {};
public:

    explicit Viewport(VkExtent2D size): Viewport((float)size.width, (float)size.height) {}

    explicit Viewport(float width, float height);

    Viewport& set_position(float x, float y) {
        m_viewport.x = x;
        m_viewport.y = y;
        return *this;
    }

    Viewport& set_size(float width, float height) {
        m_viewport.width = width;
        m_viewport.height = height;
        return *this;
    }

    Viewport& set_depth_limits(float min, float max) {
        m_viewport.minDepth = min;
        m_viewport.maxDepth = max;
        return *this;
    }

    const VkViewport& get_viewport() const { return m_viewport; }
};

}