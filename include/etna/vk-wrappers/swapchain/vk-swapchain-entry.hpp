#pragma once

#include <etna/vk-wrappers/image/view/vk-image-view.hpp>
#include <etna/vk-wrappers/framebuffer.hpp>

namespace VK {

struct SwapchainEntry {
    ImageView m_image_view {};
    Framebuffer m_framebuffer {};
};

}