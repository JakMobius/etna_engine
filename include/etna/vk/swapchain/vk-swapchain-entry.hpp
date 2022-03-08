#pragma once

#include <etna/vk/image/view/vk-image-view.hpp>
#include <etna/vk/framebuffer.hpp>

namespace VK {

struct SwapchainEntry {
    ImageView m_image_view {};
    Framebuffer m_framebuffer {};
};

}