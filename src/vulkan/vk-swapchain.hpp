#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-surface-context.hpp"
#include "vk-swap-chain-support-details.hpp"

namespace VK {

class Swapchain {

    SurfaceContext* m_surface_context;
    VkSwapchainKHR m_handle = nullptr;

    VkFormat m_image_format {};
    VkExtent2D m_extent {};
    std::vector<VkImage> m_images {};

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

        vkGetSwapchainImagesKHR(device->get_handle(), m_handle, &image_count, nullptr);
        m_images.resize(image_count);
        vkGetSwapchainImagesKHR(device->get_handle(), m_handle, &image_count, m_images.data());

        m_image_format = surface_format.format;
    }

    void destroy() {
        if(m_handle) {
            vkDestroySwapchainKHR(m_surface_context->get_device()->get_handle(), m_handle, nullptr);
            m_handle = nullptr;
        }
    }

    VkSwapchainKHR get_handle() { return m_handle; };

    VkFormat get_image_format() { return m_image_format; };
    VkExtent2D get_extent() { return m_extent; };
    const std::vector<VkImage>& get_images() { return m_images; };
    int get_image_count() { return m_images.size(); };
};

}