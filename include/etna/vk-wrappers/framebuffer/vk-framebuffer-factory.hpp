#pragma once

#include <etna/vk-wrappers/render-pass/vk-render-pass.hpp>
#include <etna/vk-wrappers/image/view/vk-image-view.hpp>
#include "vk-framebuffer.hpp"

namespace VK {

class FramebufferFactory {
    VkFramebufferCreateInfo m_description {};
    std::vector<UnownedImageView> m_attachments {};

public:
    FramebufferFactory();

    FramebufferFactory& set_size(const VkExtent2D& size) {
        m_description.width = size.width;
        m_description.height = size.height;
        return *this;
    }

    FramebufferFactory& set_layers(uint32_t layers) {
        m_description.layers = layers;
        return *this;
    }

    Framebuffer create(const RenderPass& render_pass);

    std::vector<UnownedImageView>& get_attachments() { return m_attachments; }
    uint32_t get_layers() const { return m_description.layers; }
    VkExtent2D get_size() const { return { m_description.width, m_description.height }; }
};

}