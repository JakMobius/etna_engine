#pragma once

#include "../render-pass/vk-render-pass.hpp"
#include "../image/view/vk-image-view.hpp"
#include "../framebuffer.hpp"

namespace VK {

class FramebufferFactory {
    VkFramebufferCreateInfo m_description {};
    std::vector<UnownedImageView> m_attachments {};

public:
    FramebufferFactory() {
        m_description.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    }

    void set_size(const VkExtent2D& size) {
        m_description.width = size.width;
        m_description.height = size.height;
    }

    void set_layers(uint32_t layers) {
        m_description.layers = layers;
    }

    Framebuffer create(const RenderPass& render_pass) {
        std::vector<VkImageView> image_views {};
        image_views.reserve(m_attachments.size());

        for(auto& attachment : m_attachments) {
            image_views.push_back(attachment.get_handle());
        }

        m_description.attachmentCount = image_views.size();
        m_description.pAttachments = image_views.data();
        m_description.renderPass = render_pass.get_handle();

        VkFramebuffer handle = nullptr;

        if (vkCreateFramebuffer(render_pass.get_device()->get_handle(), &m_description, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer");
        }

        m_description.attachmentCount = 0;
        m_description.pAttachments = nullptr;

        return { render_pass.get_device(), handle };
    }

    std::vector<UnownedImageView>& get_attachments() { return m_attachments; }
    uint32_t get_layers() const { return m_description.layers; }
    VkExtent2D get_size() const { return { m_description.width, m_description.height }; }
};

}