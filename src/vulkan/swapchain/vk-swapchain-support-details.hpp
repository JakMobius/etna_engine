#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "../device/vk-physical-device.hpp"

namespace VK {

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR m_capabilities {};
    std::vector<VkSurfaceFormatKHR> m_formats {};
    std::vector<VkPresentModeKHR> m_present_modes {};
    VK::UnownedSurface m_surface;
    const VK::PhysicalDevice* m_device;

    SwapChainSupportDetails(const VK::PhysicalDevice* device, const VK::UnownedSurface& surface);

    void get_present_modes();

    void get_capabilities();

    void get_formats();

    bool is_complete() const {
        return !m_formats.empty() && !m_present_modes.empty();
    }

    VkSurfaceFormatKHR choose_best_format();

    VkPresentModeKHR choose_best_present_mode();

    VkExtent2D choose_best_swap_extent(uint32_t window_width, uint32_t window_height) const;

    uint32_t get_optimal_chain_image_count() const;
};

}