#pragma once

#include "../vulkan/framebuffer.hpp"

namespace Etna {

class FramebufferManager {
public:
    virtual ~FramebufferManager() = default;
    virtual VK::Framebuffer create_framebuffer(const VK::ImageView& image_view, VkExtent2D size, const VK::RenderPass& render_pass) = 0;
};

}