//
// Created by Артем on 17.02.2022.
//

#include "vk-swapchain-images.hpp"
#include "../framebuffer.hpp"
#include "../image/view/vk-image-view-factory.hpp"

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

void VK::SwapchainImages::create_entries(const std::vector<VK::UnownedImage> &images, const VK::RenderPass &render_pass) {
    for(auto& image : images) {
        m_entries.emplace_back(image);
        auto& entry = m_entries.back();

        entry.m_swapchain = this;
        entry.create_image_view();
        entry.create_framebuffer(render_pass);
    }
}

void VK::SwapchainImages::create(uint32_t width, uint32_t height) {
    SwapChainSupportDetails swap_chain_support(m_surface_context->get_device()->get_physical_device(), m_surface_context->get_surface());

    m_extent = { width, height };

    VkSurfaceFormatKHR surface_format = swap_chain_support.choose_best_format();
    VkExtent2D extent = swap_chain_support.choose_best_swap_extent(width, height);
    uint32_t image_count = swap_chain_support.get_optimal_chain_image_count();

    VK::SwapchainFactory factory;
    factory.set_surface(m_surface_context->get_surface());
    factory.set_min_image_count(image_count);
    factory.set_image_format(surface_format.format);
    factory.set_image_color_space(surface_format.colorSpace);
    factory.set_image_extent(extent);
    factory.set_image_usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    factory.set_pre_transform(swap_chain_support.m_capabilities.currentTransform);
    factory.set_composite_alpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR);
    factory.set_clipped(VK_TRUE);

    auto queues = m_surface_context->get_queue_families();

    if (queues.size() > 1) {
        factory.set_image_sharing_mode(VK_SHARING_MODE_CONCURRENT);
        factory.get_queue_family_indices().assign(queues.begin(), queues.end());
    } else {
        factory.set_image_sharing_mode(VK_SHARING_MODE_EXCLUSIVE);
    }

    m_swapchain = factory.create(m_surface_context->get_device());
    m_image_format = surface_format.format;
}

void VK::SwapchainImages::destroy() {
    if(!m_swapchain) return;
    m_entries.clear();
    m_swapchain.destroy();
}
