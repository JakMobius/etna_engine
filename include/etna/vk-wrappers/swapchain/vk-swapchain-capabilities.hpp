#pragma once

#include <etna/volk.hpp>
#include <algorithm>

namespace VK {

class SwapchainCapabilities {
    VkSurfaceCapabilitiesKHR m_description;

public:
    SwapchainCapabilities() = default;

    uint32_t get_min_image_count()                           const { return m_description.minImageCount; }
    uint32_t get_max_image_count()                           const { return m_description.maxImageCount; }
    VkExtent2D get_current_extent()                          const { return m_description.currentExtent; }
    VkExtent2D get_min_image_extent()                        const { return m_description.minImageExtent; }
    VkExtent2D get_max_image_extent()                        const { return m_description.maxImageExtent; }
    uint32_t get_max_image_array_layers()                    const { return m_description.maxImageArrayLayers; }
    VkSurfaceTransformFlagsKHR get_supported_transforms()    const { return m_description.supportedTransforms; }
    VkSurfaceTransformFlagBitsKHR get_current_transform()    const { return m_description.currentTransform; }
    VkCompositeAlphaFlagsKHR get_supported_composite_alpha() const { return m_description.supportedCompositeAlpha; }
    VkImageUsageFlags get_supported_usage_flags()            const { return m_description.supportedUsageFlags; }

    uint32_t get_optimal_chain_image_count() const {
        uint32_t image_count = m_description.minImageCount + 1;
        if(m_description.maxImageCount > 0 && image_count > m_description.maxImageCount) {
            image_count = m_description.maxImageCount;
        }

        return image_count;
    }

    VkExtent2D clamp_image_extent(uint32_t window_width, uint32_t window_height) const {
        if(m_description.currentExtent.width != UINT32_MAX) {
            return m_description.currentExtent;
        } else {
            VkExtent2D actualExtent = {
                std::clamp(window_width, m_description.minImageExtent.width, m_description.minImageExtent.width),
                std::clamp(window_height, m_description.maxImageExtent.height, m_description.minImageExtent.height),
            };

            return actualExtent;
        }
    }

    VkSurfaceCapabilitiesKHR& get_description() { return m_description; }
};

}