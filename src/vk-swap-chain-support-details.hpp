#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

struct VkSwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR m_capabilities {};
    std::vector<VkSurfaceFormatKHR> m_formats {};
    std::vector<VkPresentModeKHR> m_present_modes {};
    VkPhysicalDevice m_device;
    VkSurfaceKHR m_surface;

    explicit VkSwapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface):
        m_device(device), m_surface(surface) {

        get_capabilities();
        get_formats();
        get_present_modes();

    }

    void get_present_modes() {
        uint32_t present_mode_count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_device, m_surface, &present_mode_count, nullptr);

        if (present_mode_count != 0) {
            m_present_modes.resize(present_mode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(m_device, m_surface, &present_mode_count, m_present_modes.data());
        }
    }

    void get_capabilities() {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device, m_surface, &m_capabilities);
    }

    void get_formats() {
        uint32_t format_count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_device, m_surface, &format_count, nullptr);

        if (format_count != 0) {
            m_formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(m_device, m_surface, &format_count, m_formats.data());
        }
    }

    bool is_complete() {
        return !m_formats.empty() && !m_formats.empty();
    }

    VkSurfaceFormatKHR choose_best_format() {
        for (const auto& available_format : m_formats) {
            if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return available_format;
            }
        }

        return m_formats[0];
    }

    VkPresentModeKHR choose_best_present_mode() {
        for (const auto& available_present_mode : m_present_modes) {
            if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return available_present_mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D choose_best_swap_extent(int window_width, int window_height) {
        if (m_capabilities.currentExtent.width != UINT32_MAX) {
            return m_capabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = {
                std::max(std::min((uint32_t)window_width, m_capabilities.maxImageExtent.width), m_capabilities.minImageExtent.width),
                std::max(std::min((uint32_t)window_height, m_capabilities.maxImageExtent.height), m_capabilities.minImageExtent.height),
            };

            return actualExtent;
        }
    }

    uint32_t get_optimal_chain_image_count() {
        uint32_t image_count = m_capabilities.minImageCount + 1;
        if (m_capabilities.maxImageCount > 0 && image_count > m_capabilities.maxImageCount) {
            image_count = m_capabilities.maxImageCount;
        }

        return image_count;
    }
};