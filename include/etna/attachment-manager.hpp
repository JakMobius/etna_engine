#pragma once

#include <etna/vk-wrappers/framebuffer.hpp>

namespace Etna {

class AttachmentManager {
public:
    virtual ~AttachmentManager() = default;
    virtual VK::Framebuffer create_framebuffer(const VK::ImageView& image_view, VkExtent2D size, const VK::RenderPass& render_pass) = 0;
};

}