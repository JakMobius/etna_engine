#pragma once

#include <etna/attachment-manager.hpp>

namespace Etna {

class BasicAttachmentManager : public AttachmentManager {

    VK::UnownedImageView m_color_image_view {};
    VK::UnownedImageView m_depth_image_view {};

public:
    BasicAttachmentManager() {}

    VK::Framebuffer create_framebuffer(const VK::ImageView& image_view, VkExtent2D size, const VK::RenderPass& render_pass) override {
        VK::FramebufferFactory framebuffer_factory;
        framebuffer_factory.set_size(size);

        auto& attachments = framebuffer_factory.get_attachments();

        if(m_color_image_view) attachments.push_back(m_color_image_view);
        if(m_depth_image_view) attachments.push_back(m_depth_image_view);

        attachments.push_back(image_view);

        return framebuffer_factory.create(render_pass);
    }

    uint32_t get_image_attachment_index() {
        uint32_t result = 0;
        if(m_color_image_view) result++;
        if(m_depth_image_view) result++;
        return result;
    }

    uint32_t get_depth_attachment_index() {
        if(!m_depth_image_view) return UINT32_MAX;
        return 1;
    }

    uint32_t get_color_attachment_index() {
        if(!m_color_image_view) return UINT32_MAX;
        return 0;
    }

    void set_color_image_view(const VK::UnownedImageView& image_view) { m_color_image_view = image_view; }
    void set_depth_image_view(const VK::UnownedImageView& image_view) { m_depth_image_view = image_view; }
};

}