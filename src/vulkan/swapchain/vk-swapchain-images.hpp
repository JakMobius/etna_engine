#pragma once

namespace VK {

class SwapchainEntry;
class SwapchainImages;

}

#include <vulkan/vulkan_core.h>
#include "../image/vk-image.hpp"
#include "../framebuffer.hpp"
#include "../vk-surface-context.hpp"
#include "../swapchain.hpp"

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

    void create_entries(const std::vector<VK::UnownedImage>& images, const RenderPass& render_pass);

public:
    explicit SwapchainImages(SurfaceContext* surface_context): m_surface_context(surface_context) {}

    ~SwapchainImages() { destroy(); }

    void create(uint32_t width, uint32_t height);

    void create_images(const RenderPass& render_pass) {
        create_entries(m_swapchain.get_swapchain_images(), render_pass);
    }

    void destroy();

    VK::Swapchain& get_swapchain() { return m_swapchain; };

    std::vector<UnownedImageView>& get_framebuffer_attachments() { return m_framebuffer_attachments; }
    SurfaceContext* get_surface_context() { return m_surface_context; }
    VkFormat get_image_format() { return m_image_format; };
    VkExtent2D get_extent() { return m_extent; };
    std::vector<SwapchainEntry>& get_entries() { return m_entries; };
    int get_image_count() { return m_entries.size(); };
};

}