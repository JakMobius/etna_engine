#pragma once

namespace VK {

class SwapchainEntry;
class SwapchainImages;

}

#include <vulkan/vulkan_core.h>
#include "../vk-surface-context.hpp"
#include "vk-swapchain-support-details.hpp"
#include "../image/view/vk-image-view.hpp"
#include "../framebuffer/vk-framebuffer.hpp"
#include "../render-pass/vk-render-pass.hpp"
#include "vk-swapchain-factory.hpp"

namespace VK {

struct SwapchainEntry {
    SwapchainImages* m_swapchain = nullptr;
    UnownedImage m_image;
    ImageView m_image_view {};
    Framebuffer m_framebuffer {};

    explicit SwapchainEntry(const UnownedImage& image): m_image(image) {};

    void create_image_view();
    void create_framebuffer(const RenderPass& render_pass);
};

class SwapchainImages {

    SurfaceContext* m_surface_context;
    Swapchain m_swapchain;

    VkFormat m_image_format {};
    VkExtent2D m_extent {};

    std::vector<SwapchainEntry> m_entries {};
    std::vector<UnownedImageView> m_framebuffer_attachments {};

    void create_entries(const std::vector<VK::UnownedImage>& images, const RenderPass& render_pass) {
        for(auto& image : images) {
            m_entries.emplace_back(image);
            auto& entry = m_entries.back();

            entry.m_swapchain = this;
            entry.create_image_view();
            entry.create_framebuffer(render_pass);
        }
    }

public:
    explicit SwapchainImages(SurfaceContext* surface_context): m_surface_context(surface_context) {}

    ~SwapchainImages() { destroy(); }

    void create(uint32_t width, uint32_t height) {
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

    void create_images(const RenderPass& render_pass) {
        create_entries(m_swapchain.get_swapchain_images(), render_pass);
    }

    void destroy() {
        if(!m_swapchain) return;
        m_entries.clear();
        m_swapchain.destroy();
    }

    VK::Swapchain& get_swapchain() { return m_swapchain; };

    std::vector<UnownedImageView>& get_framebuffer_attachments() { return m_framebuffer_attachments; }
    SurfaceContext* get_surface_context() { return m_surface_context; }
    VkFormat get_image_format() { return m_image_format; };
    VkExtent2D get_extent() { return m_extent; };
    std::vector<SwapchainEntry>& get_entries() { return m_entries; };
    int get_image_count() { return m_entries.size(); };
};

}