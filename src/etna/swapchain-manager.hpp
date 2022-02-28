#pragma once

#include <set>
#include <vulkan/vulkan_core.h>
#include "../vulkan/vk-surface.hpp"
#include "../vulkan/swapchain/vk-swapchain.hpp"
#include "../vulkan/framebuffer/vk-framebuffer.hpp"
#include "../vulkan/swapchain/vk-swapchain-factory.hpp"
#include "../vulkan/swapchain/vk-swapchain-support-details.hpp"
#include "../vulkan/image/view/vk-image-view-factory.hpp"
#include "../vulkan/framebuffer/vk-framebuffer-factory.hpp"
#include "framebuffer-manager.hpp"

namespace Etna {

struct SwapchainImage {
    VK::ImageView m_image_view {};
    VK::Framebuffer m_framebuffer {};
};

class SwapchainManager {
    VK::Device* m_device = nullptr;
    VK::UnownedSurface m_surface {};
    VK::Swapchain m_swapchain;
    std::vector<SwapchainImage> m_swapchain_images {};
    std::set<uint32_t> m_family_indices {};
    VkExtent2D m_swapchain_extent {};
    VkFormat m_swapchain_image_format;

    FramebufferManager* m_framebuffer_manager = nullptr;

protected:

    virtual void configure_swapchain_image_view(VK::ImageViewFactory& image_view_factory, VK::UnownedImage& swapchain_image) {
        image_view_factory.set_format(m_swapchain_image_format);
        image_view_factory.get_subresource_range().aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    void configure_swapchain_image(SwapchainImage& image, VK::UnownedImage& swapchain_image, const VK::RenderPass& render_pass) {
        VK::ImageViewFactory image_view_factory;
        configure_swapchain_image_view(image_view_factory, swapchain_image);
        image.m_image_view = image_view_factory.create(m_device, swapchain_image);
        image.m_framebuffer = m_framebuffer_manager->create_framebuffer(image.m_image_view, m_swapchain_extent, render_pass);
    }

    virtual void configure_swapchain(VK::SwapchainFactory& factory, uint32_t width, uint32_t height) {
        VK::SwapChainSupportDetails swap_chain_support(m_device->get_physical_device(), m_surface);

        VkSurfaceFormatKHR surface_format = swap_chain_support.choose_best_format();
        m_swapchain_extent = swap_chain_support.choose_best_swap_extent(width, height);
        uint32_t image_count = swap_chain_support.get_optimal_chain_image_count();

        factory.set_surface(m_surface);
        factory.set_min_image_count(image_count);
        factory.set_image_format(surface_format.format);
        factory.set_image_color_space(surface_format.colorSpace);
        factory.set_image_extent(m_swapchain_extent);
        factory.set_image_usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        factory.set_pre_transform(swap_chain_support.m_capabilities.currentTransform);
        factory.set_composite_alpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR);
        factory.set_clipped(VK_TRUE);

        if (m_family_indices.size() > 1) {
            factory.set_image_sharing_mode(VK_SHARING_MODE_CONCURRENT);
            factory.get_queue_family_indices().assign(m_family_indices.begin(), m_family_indices.end());
        } else {
            factory.set_image_sharing_mode(VK_SHARING_MODE_EXCLUSIVE);
        }
    }

public:
    explicit SwapchainManager(const VK::UnownedSurface& surface, VK::Device* device): m_surface(surface), m_device(device) {}

    void create_swapchain(uint32_t width, uint32_t height) {
        VK::SwapchainFactory factory;

        configure_swapchain(factory, width, height);

        m_swapchain_image_format = factory.get_image_format();
        m_swapchain_extent = factory.get_image_extent();

        m_swapchain = factory.create(m_device);
    }

    void create_swapchain_images(const VK::RenderPass& render_pass) {
        auto swapchain_images = m_swapchain.get_swapchain_images();

        for(auto& swapchain_image : swapchain_images) {
            m_swapchain_images.emplace_back();
            configure_swapchain_image(m_swapchain_images.back(), swapchain_image, render_pass);
        }
    }

    void destroy_swapchain() {
        m_swapchain.destroy();
        m_swapchain_images.clear();
    }

    void destroy() {
        destroy_swapchain();
    }

    void set_framebuffer_manager(FramebufferManager* framebuffer_manager) {
        m_framebuffer_manager = framebuffer_manager;
    }

    VkExtent2D get_swapchain_extent() const { return m_swapchain_extent; }
    const VkFormat get_swapchain_image_format() const { return m_swapchain_image_format; }
    const VK::Swapchain& get_swapchain() const { return m_swapchain; }
    const std::vector<SwapchainImage>& get_swapchain_images() const { return m_swapchain_images; }
};

}