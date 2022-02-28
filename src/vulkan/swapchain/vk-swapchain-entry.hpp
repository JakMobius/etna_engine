#pragma once

#include "../image/view/vk-image-view.hpp"
#include "../framebuffer.hpp"

namespace VK {

struct SwapchainEntry {
    ImageView m_image_view {};
    Framebuffer m_framebuffer {};
};

}