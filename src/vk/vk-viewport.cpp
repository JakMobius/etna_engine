
#include <etna/vk-wrappers/vk-viewport.hpp>

VK::Viewport::Viewport(float width, float height) {
    m_viewport.x = 0.0f;
    m_viewport.y = 0.0f;
    m_viewport.width = width;
    m_viewport.height = height;
    m_viewport.minDepth = 0.0f;
    m_viewport.maxDepth = 1.0f;
}
