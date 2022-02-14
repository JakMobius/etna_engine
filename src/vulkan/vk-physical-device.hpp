#pragma once

namespace VK {
class PhysicalDevice;
}

#include <vulkan/vulkan_core.h>
#include <memory>
#include <iostream>
#include <set>
#include "codes/vk-version-code.hpp"
#include "codes/vk-physical-device-type-code.hpp"
#include "vk-queue-family-indices.hpp"

namespace VK {

class PhysicalDevice {
    VkPhysicalDevice m_handle;

    mutable std::unique_ptr<VkPhysicalDeviceMemoryProperties> m_mem_properties = nullptr;
    mutable std::unique_ptr<VkPhysicalDeviceProperties> m_physical_properties = nullptr;
    mutable std::unique_ptr<VkPhysicalDeviceFeatures> m_physical_features = nullptr;
    mutable std::unique_ptr<QueueFamilyIndices> m_queue_family_indices = nullptr;


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

    const QueueFamilyIndices* get_queue_family_indices() const;

    void get_format_properties(VkFormatProperties* properties, VkFormat format);

    VkSampleCountFlagBits get_max_usable_sample_count() const;

    uint32_t get_suitable_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const;

    void print_description();

    bool supports_extensions(std::vector<const char*> extensions) const;
    VkFormat find_supported_format(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkPhysicalDevice get_handle() const { return m_handle; }
};

}