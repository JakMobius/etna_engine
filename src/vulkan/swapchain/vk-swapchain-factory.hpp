#pragma once

#include "../device/vk-device.hpp"
#include "vk-swapchain.hpp"

namespace VK {

class SwapchainFactory {
    VkSwapchainCreateInfoKHR m_description {};
    std::vector<uint32_t> m_queue_family_indices {};
public:
    SwapchainFactory();

    void set_flags(VkSwapchainCreateFlagsKHR flags) {
        m_description.flags = flags;
    }

    void set_surface(const VK::SurfaceBase& surface) {
        m_description.surface = surface.get_handle();
    }

    void set_min_image_count(uint32_t min_image_count) {
        m_description.minImageCount = min_image_count;
    }

    void set_image_format(VkFormat image_format) {
        m_description.imageFormat = image_format;
    }

    void set_image_color_space(VkColorSpaceKHR image_color_space) {
        m_description.imageColorSpace = image_color_space;
    }

    void set_image_extent(VkExtent2D image_extent) {
        m_description.imageExtent = image_extent;
    }

    void set_image_array_layers(uint32_t image_array_layers) {
        m_description.imageArrayLayers = image_array_layers;
    }

    void set_image_usage(VkImageUsageFlags image_usage) {
        m_description.imageUsage = image_usage;
    }

    void set_image_sharing_mode(VkSharingMode image_sharing_mode) {
        m_description.imageSharingMode = image_sharing_mode;
    }

    void set_pre_transform(VkSurfaceTransformFlagBitsKHR pre_transform) {
        m_description.preTransform = pre_transform;
    }

    void set_composite_alpha(VkCompositeAlphaFlagBitsKHR composite_alpha) {
        m_description.compositeAlpha = composite_alpha;
    }

    void set_present_mode(VkPresentModeKHR present_mode) {
        m_description.presentMode = present_mode;
    }

    void set_clipped(VkBool32 clipped) {
        m_description.clipped = clipped;
    }

    void set_old_swapchain(VkSwapchainKHR old_swapchain) {
        m_description.oldSwapchain = old_swapchain;
    }

    Swapchain create(Device* device);

    VkSwapchainCreateFlagsKHR     get_flags()                    const { return m_description.flags; }
    VkSurfaceKHR                  get_surface()                  const { return m_description.surface; }
    uint32_t                      get_min_image_count()          const { return m_description.minImageCount; }
    VkFormat                      get_image_format()             const { return m_description.imageFormat; }
    VkColorSpaceKHR               get_image_color_space()        const { return m_description.imageColorSpace; }
    VkExtent2D                    get_image_extent()             const { return m_description.imageExtent; }
    uint32_t                      get_image_array_layers()       const { return m_description.imageArrayLayers; }
    VkImageUsageFlags             get_image_usage()              const { return m_description.imageUsage; }
    VkSharingMode                 get_image_sharing_mode()       const { return m_description.imageSharingMode; }
    VkSurfaceTransformFlagBitsKHR get_pre_transform()            const { return m_description.preTransform; }
    VkCompositeAlphaFlagBitsKHR   get_composite_alpha()          const { return m_description.compositeAlpha; }
    VkPresentModeKHR              get_present_mode()             const { return m_description.presentMode; }
    VkBool32                      get_clipped()                  const { return m_description.clipped; }
    VkSwapchainKHR                get_old_swapchain()            const { return m_description.oldSwapchain; }
    std::vector<uint32_t>&        get_queue_family_indices()           { return m_queue_family_indices; }
};

}