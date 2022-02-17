#pragma once

namespace VK {

class SwapchainEntry;
class Swapchain;

}

#include <vulkan/vulkan_core.h>
#include "vk-surface-context.hpp"
#include "vk-swap-chain-support-details.hpp"
#include "vk-image-view.hpp"
#include "vk-framebuffer.hpp"

namespace VK {

struct SwapchainEntry {
    Swapchain* m_swapchain = nullptr;
    VkImage m_image {};
    std::unique_ptr<ImageView> m_image_view {};
    std::unique_ptr<Framebuffer> m_framebuffer {};
    std::unique_ptr<CommandBuffer> m_command_buffer {};

    void create_image_view();
    void create_framebuffer(VkRenderPass render_pass);
    void destroy();
};

class Swapchain {

    SurfaceContext* m_surface_context;
    VkSwapchainKHR m_handle = nullptr;

    VkFormat m_image_format {};
    VkExtent2D m_extent {};

    std::vector<SwapchainEntry> m_entries {};
    std::vector<ImageView*> m_framebuffer_attachments {};

    void create_entries(const std::vector<VkImage>& images, VkRenderPass render_pass) {
        for(auto image : images) {
            m_entries.emplace_back();
            auto& entry = m_entries.back();

            entry.m_swapchain = this;
            entry.m_image = image;
            entry.create_image_view();
            entry.create_framebuffer(render_pass);

        }
    }

public:
    explicit Swapchain(SurfaceContext* surface_context): m_surface_context(surface_context) {}

    ~Swapchain() {
        destroy();
    }

    void create(uint32_t width, uint32_t height) {
        auto device = m_surface_context->get_device();
        SwapChainSupportDetails swap_chain_support(m_surface_context);

        m_extent = { width, height };

        VkSurfaceFormatKHR surface_format = swap_chain_support.choose_best_format();
        VkPresentModeKHR present_mode = swap_chain_support.choose_best_present_mode();
        VkExtent2D extent = swap_chain_support.choose_best_swap_extent(width, height);

        uint32_t image_count = swap_chain_support.get_optimal_chain_image_count();

        VkSwapchainCreateInfoKHR create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = m_surface_context->get_surface();
        create_info.minImageCount = image_count;
        create_info.imageFormat = surface_format.format;
        create_info.imageColorSpace = surface_format.colorSpace;
        create_info.imageExtent = extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queue_family_indices[] = {
                (uint32_t) m_surface_context->get_graphics_queue_index(),
                (uint32_t) m_surface_context->get_present_queue_index()
        };

        if (m_surface_context->get_graphics_queue_index() != m_surface_context->get_present_queue_index()) {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_family_indices;
        } else {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            create_info.queueFamilyIndexCount = 0;
            create_info.pQueueFamilyIndices = nullptr;
        }

        create_info.preTransform = swap_chain_support.m_capabilities.currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.clipped = VK_TRUE;

        create_info.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_surface_context->get_device()->get_handle(), &create_info, nullptr, &m_handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain");
        }

        m_image_format = surface_format.format;
    }

    void create_images(VkRenderPass render_pass) {
        auto device = m_surface_context->get_device();

        uint32_t image_count;
        std::vector<VkImage> images {};
        vkGetSwapchainImagesKHR(device->get_handle(), m_handle, &image_count, nullptr);
        images.resize(image_count);
        vkGetSwapchainImagesKHR(device->get_handle(), m_handle, &image_count, images.data());

        create_entries(images, render_pass);
    }

    void destroy() {
        if(m_handle) {
            for(auto& entry : m_entries) entry.destroy();
            m_entries.clear();

            vkDestroySwapchainKHR(m_surface_context->get_device()->get_handle(), m_handle, nullptr);
            m_handle = nullptr;
        }
    }

    VkSwapchainKHR get_handle() { return m_handle; };

    std::vector<ImageView*>& get_framebuffer_attachments() { return m_framebuffer_attachments; }
    SurfaceContext* get_surface_context() { return m_surface_context; }
    VkFormat get_image_format() { return m_image_format; };
    VkExtent2D get_extent() { return m_extent; };
    std::vector<SwapchainEntry>& get_entries() { return m_entries; };
    int get_image_count() { return m_entries.size(); };
};

}