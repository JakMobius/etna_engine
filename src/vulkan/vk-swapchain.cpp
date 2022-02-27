//
// Created by Артем on 17.02.2022.
//

#include "vk-swapchain.hpp"
#include "framebuffer.hpp"
#include "image/view/vk-image-view-factory.hpp"

void VK::SwapchainEntry::destroy() {


}

void VK::SwapchainEntry::create_image_view() {
    ImageViewFactory factory;
    factory.set_format(m_swapchain->get_image_format());
    factory.get_subresource_range().aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_image_view = factory.create(m_swapchain->get_surface_context()->get_device(), m_image);
}

void VK::SwapchainEntry::create_framebuffer(const VK::RenderPass& render_pass) {

    VK::FramebufferFactory factory;
    factory.set_size(m_swapchain->get_extent());
    auto& attachments = factory.get_attachments();

    for(auto& attachment : m_swapchain->get_framebuffer_attachments()) {
        factory.get_attachments().push_back(attachment);
    }
    attachments.push_back(m_image_view.unowned_copy());

    m_framebuffer = factory.create(render_pass);
}