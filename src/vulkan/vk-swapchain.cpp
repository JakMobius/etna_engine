//
// Created by Артем on 17.02.2022.
//

#include "vk-swapchain.hpp"

void VK::SwapchainEntry::destroy() {


}

void VK::SwapchainEntry::create_image_view() {
    m_image_view = std::make_unique<ImageView>(m_image, m_swapchain->get_surface_context()->get_device());
    m_image_view->get_subresource_range().aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_image_view->set_format(m_swapchain->get_image_format());
    m_image_view->set_view_type(VK_IMAGE_VIEW_TYPE_2D);
    m_image_view->create();
}

void VK::SwapchainEntry::create_framebuffer(VkRenderPass render_pass) {
    auto device = m_swapchain->get_surface_context()->get_device();

    m_framebuffer = std::make_unique<Framebuffer>(device, render_pass);
    auto& attachments = m_framebuffer->get_attachments();

    for(auto attachment : m_swapchain->get_framebuffer_attachments()) {
        attachments.push_back(attachment);
    }
    attachments.push_back(m_image_view.get());

    m_framebuffer->set_size(m_swapchain->get_extent());
    m_framebuffer->create();
}