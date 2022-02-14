#pragma once

namespace VK {
class PhysicalDevice;
}

#include <vulkan/vulkan_core.h>
#include <memory>
#include <iostream>
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

    PhysicalDevice& operator=(const PhysicalDevice& copy_assign) {
        if(this == &copy_assign) return *this;

        m_handle = copy_assign.m_handle;
        m_mem_properties = nullptr;
        m_physical_properties = nullptr;
        m_physical_features = nullptr;
        m_queue_family_indices = nullptr;
        return *this;
    };

    PhysicalDevice& operator=(PhysicalDevice&& move_assign) noexcept {
        if(this == &move_assign) return *this;

        m_handle = move_assign.m_handle;
        m_mem_properties = std::move(move_assign.m_mem_properties);
        m_physical_properties = std::move(move_assign.m_physical_properties);
        m_physical_features = std::move(move_assign.m_physical_features);
        m_queue_family_indices = std::move(move_assign.m_queue_family_indices);

        return *this;
    };

    const VkPhysicalDeviceMemoryProperties* get_memory_properties() const {
        if(!m_mem_properties) {
            m_mem_properties = std::make_unique<VkPhysicalDeviceMemoryProperties>();
            vkGetPhysicalDeviceMemoryProperties(m_handle, m_mem_properties.get());
        }
        return m_mem_properties.get();
    }

    const VkPhysicalDeviceProperties* get_physical_properties() const {
        if(!m_physical_properties) {
            m_physical_properties = std::make_unique<VkPhysicalDeviceProperties>();
            vkGetPhysicalDeviceProperties(m_handle, m_physical_properties.get());
        }
        return m_physical_properties.get();
    }

    const VkPhysicalDeviceFeatures* get_physical_features() const {
        if(!m_physical_features) {
            m_physical_features = std::make_unique<VkPhysicalDeviceFeatures>();
            vkGetPhysicalDeviceFeatures(m_handle, m_physical_features.get());
        }
        return m_physical_features.get();
    }

    const QueueFamilyIndices* get_queue_family_indices() const {
        if(!m_queue_family_indices) {
            m_queue_family_indices = std::make_unique<QueueFamilyIndices>(this);
        }
        return m_queue_family_indices.get();
    }

    void get_format_properties(VkFormatProperties* properties, VkFormat format) {
        vkGetPhysicalDeviceFormatProperties(m_handle, format, properties);
    }

    VkSampleCountFlagBits get_max_usable_sample_count() const {
        VkSampleCountFlags counts = get_physical_properties()->limits.framebufferColorSampleCounts &
                                    get_physical_properties()->limits.framebufferDepthSampleCounts;

        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
    }

    void print_description() {
        auto properties =get_physical_properties();

        std::cout << "VK::PhysicalDevice[" << properties->deviceName << "]:\n";
        std::cout << "\tapiVersion: " << VK::VersionCode(properties->apiVersion) << "\n";
        std::cout << "\tdriverVersion: " << VK::VersionCode(properties->driverVersion) << "\n";
        std::cout << "\tvendorID: " << properties->vendorID << "\n";
        std::cout << "\tdeviceID: " << properties->deviceID << "\n";
        std::cout << "\tdeviceType: " << VK::PhysicalDeviceTypeCode(properties->deviceType) << "\n";
    }

    VkPhysicalDevice get_handle() const { return m_handle; }
};

}