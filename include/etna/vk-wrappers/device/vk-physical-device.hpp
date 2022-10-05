#pragma once

namespace VK {
class PhysicalDevice;
}

#include <memory>
#include <etna/volk.hpp>
#include <etna/vk-wrappers/queue/vk-queue-family-indices.hpp>
#include <etna/vk-wrappers/vk-surface.hpp>
#include <etna/vk-wrappers/swapchain/vk-swapchain-capabilities.hpp>

namespace VK {

class PhysicalDevice {
    VkPhysicalDevice m_handle;

    mutable std::unique_ptr<VkPhysicalDeviceMemoryProperties> m_mem_properties = nullptr;
    mutable std::unique_ptr<VkPhysicalDeviceProperties> m_physical_properties = nullptr;
    mutable std::unique_ptr<VkPhysicalDeviceFeatures> m_physical_features = nullptr;
    mutable std::unique_ptr<DeviceQueueFamilies> m_queue_family_indices = nullptr;

public:
    PhysicalDevice(): m_handle(nullptr) {}
    explicit PhysicalDevice(VkPhysicalDevice device): m_handle(device) {}
    PhysicalDevice(PhysicalDevice&& move) noexcept { *this = std::move(move); }

    PhysicalDevice(const PhysicalDevice& copy): m_handle(copy.m_handle) {}

    PhysicalDevice& operator=(const PhysicalDevice& copy_assign);

    PhysicalDevice& operator=(PhysicalDevice&& move_assign) noexcept;

    const VkPhysicalDeviceMemoryProperties* get_memory_properties() const;

    const VkPhysicalDeviceProperties* get_physical_properties() const;

    const VkPhysicalDeviceFeatures* get_physical_features() const;

    bool has_surface_present_modes(const VK::UnownedSurface& surface) const;
    std::vector<VkPresentModeKHR> get_surface_present_modes(const VK::UnownedSurface& surface) const;
    bool supports_surface_present_mode(const VK::UnownedSurface& surface, VkPresentModeKHR mode) const;
    VK::SwapchainCapabilities get_surface_capabilities(const VK::UnownedSurface& surface) const;
    bool has_supported_surface_formats(const VK::UnownedSurface& surface) const;
    bool supports_surface_format(const VK::UnownedSurface& each, VkSurfaceFormatKHR format) const;
    std::vector<VkSurfaceFormatKHR> get_supported_surface_formats(const VK::UnownedSurface& surface) const;

    const DeviceQueueFamilies* get_queue_family_indices() const;

    void get_format_properties(VkFormatProperties* properties, VkFormat format);

    VkSampleCountFlagBits get_max_usable_sample_count() const;

    uint32_t get_suitable_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const;

    void print_description();

    bool supports_extensions(std::vector<const char*> extensions) const;
    VkFormat find_supported_format(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkPhysicalDevice get_handle() const { return m_handle; }
};

}