
#include <etna/vk-wrappers/framebuffer/vk-framebuffer-factory.hpp>

VK::FramebufferFactory::FramebufferFactory() {
    m_description.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    m_description.layers = 1;
}

VK::Framebuffer VK::FramebufferFactory::create(const VK::RenderPass &render_pass) {
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
