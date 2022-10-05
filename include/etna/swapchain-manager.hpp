#pragma once

#include <set>
#include <etna/volk.hpp>
#include <etna/vk-wrappers/vk-surface.hpp>
#include <etna/vk-wrappers/swapchain/vk-swapchain.hpp>
#include <etna/vk-wrappers/framebuffer/vk-framebuffer.hpp>
#include <etna/vk-wrappers/swapchain/vk-swapchain-factory.hpp>
#include <etna/vk-wrappers/image/view/vk-image-view-factory.hpp>
#include <etna/vk-wrappers/framebuffer/vk-framebuffer-factory.hpp>
#include <etna/attachment-manager.hpp>

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

    AttachmentManager* m_attachment_manager = nullptr;

protected:

    virtual void configure_swapchain_image_view(VK::ImageViewFactory& image_view_factory, VK::UnownedImage& swapchain_image) {
        image_view_factory.set_format(m_swapchain_image_format);
        image_view_factory.get_subresource_range().set_aspect_mask(VK_IMAGE_ASPECT_COLOR_BIT);
    }

    void configure_swapchain_image(SwapchainImage& image, VK::UnownedImage& swapchain_image, const VK::RenderPass& render_pass) {
        VK::ImageViewFactory image_view_factory;
        configure_swapchain_image_view(image_view_factory, swapchain_image);
        image.m_image_view = image_view_factory.create(m_device, swapchain_image);
        image.m_framebuffer = m_attachment_manager->create_framebuffer(image.m_image_view, m_swapchain_extent, render_pass);
    }

    virtual void configure_swapchain(VK::SwapchainFactory& factory, uint32_t width, uint32_t height) {
        auto physical_device = m_device->get_physical_device();

        auto swapchain_capabilities = m_device->get_physical_device()->get_surface_capabilities(m_surface);

        m_swapchain_extent = swapchain_capabilities.clamp_image_extent(width, height);
        uint32_t image_count = swapchain_capabilities.get_optimal_chain_image_count();

        // TODO: not quite optimal
        VkSurfaceFormatKHR surface_format({ VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR });
        if(!physical_device->supports_surface_format(m_surface, surface_format)) {
            surface_format = physical_device->get_supported_surface_formats(m_surface)[0];
        }

        if(physical_device->supports_surface_present_mode(m_surface, VK_PRESENT_MODE_MAILBOX_KHR)) {
            factory.set_present_mode(VK_PRESENT_MODE_MAILBOX_KHR);
        } else {
            factory.set_present_mode(VK_PRESENT_MODE_FIFO_KHR);
        }

        factory.set_surface(m_surface);
        factory.set_min_image_count(image_count);
        factory.set_image_format(surface_format.format);
        factory.set_image_color_space(surface_format.colorSpace);
        factory.set_image_extent(m_swapchain_extent);
        factory.set_image_usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        factory.set_pre_transform(swapchain_capabilities.get_current_transform());
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

    void set_attachment_manager(AttachmentManager* framebuffer_manager) {
        m_attachment_manager = framebuffer_manager;
    }

    std::set<uint32_t> get_family_indices() const { return m_family_indices; }
    VkExtent2D get_swapchain_extent() const { return m_swapchain_extent; }
    const VkFormat get_swapchain_image_format() const { return m_swapchain_image_format; }
    const VK::Swapchain& get_swapchain() const { return m_swapchain; }
    const std::vector<SwapchainImage>& get_swapchain_images() const { return m_swapchain_images; }
};

}