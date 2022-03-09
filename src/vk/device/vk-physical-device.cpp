
#include <memory>
#include <iostream>
#include <set>
#include <etna/vk-wrappers/device/vk-physical-device.hpp>
#include <etna/vk-wrappers/vk-surface.hpp>
#include <etna/vk-wrappers/codes/vk-physical-device-type-code.hpp>
#include <etna/vk-wrappers/codes/vk-version-code.hpp>

const VK::DeviceQueueFamilies* VK::PhysicalDevice::get_queue_family_indices() const {
    if(!m_queue_family_indices) {
        m_queue_family_indices = std::make_unique<DeviceQueueFamilies>(this);
    }
    return m_queue_family_indices.get();
}

const VkPhysicalDeviceFeatures* VK::PhysicalDevice::get_physical_features() const {
    if(!m_physical_features) {
        m_physical_features = std::make_unique<VkPhysicalDeviceFeatures>();
        vkGetPhysicalDeviceFeatures(m_handle, m_physical_features.get());
    }
    return m_physical_features.get();
}

const VkPhysicalDeviceProperties* VK::PhysicalDevice::get_physical_properties() const {
    if(!m_physical_properties) {
        m_physical_properties = std::make_unique<VkPhysicalDeviceProperties>();
        vkGetPhysicalDeviceProperties(m_handle, m_physical_properties.get());
    }
    return m_physical_properties.get();
}

const VkPhysicalDeviceMemoryProperties* VK::PhysicalDevice::get_memory_properties() const {
    if(!m_mem_properties) {
        m_mem_properties = std::make_unique<VkPhysicalDeviceMemoryProperties>();
        vkGetPhysicalDeviceMemoryProperties(m_handle, m_mem_properties.get());
    }
    return m_mem_properties.get();
}

void VK::PhysicalDevice::get_format_properties(VkFormatProperties* properties, VkFormat format) {
    vkGetPhysicalDeviceFormatProperties(m_handle, format, properties);
}

VkSampleCountFlagBits VK::PhysicalDevice::get_max_usable_sample_count() const {
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

uint32_t VK::PhysicalDevice::get_suitable_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const {
    auto* mem_properties = get_memory_properties();

    for (uint32_t i = 0; i < mem_properties->memoryTypeCount; i++) {
        if ((type_filter & (1 << i)) && (mem_properties->memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type");
}

VK::PhysicalDevice &VK::PhysicalDevice::operator=(VK::PhysicalDevice &&move_assign) noexcept {
    if(this == &move_assign) return *this;

    m_handle = move_assign.m_handle;
    m_mem_properties = std::move(move_assign.m_mem_properties);
    m_physical_properties = std::move(move_assign.m_physical_properties);
    m_physical_features = std::move(move_assign.m_physical_features);
    m_queue_family_indices = std::move(move_assign.m_queue_family_indices);

    return *this;
}

VK::PhysicalDevice &VK::PhysicalDevice::operator=(const VK::PhysicalDevice &copy_assign) {
    if(this == &copy_assign) return *this;

    m_handle = copy_assign.m_handle;
    m_mem_properties = nullptr;
    m_physical_properties = nullptr;
    m_physical_features = nullptr;
    m_queue_family_indices = nullptr;
    return *this;
}

bool VK::PhysicalDevice::supports_extensions(std::vector<const char*> extensions) const {
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extension_count, available_extensions.data());

    std::set<std::string> required_extensions(extensions.begin(), extensions.end());

    for (const auto& extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

void VK::PhysicalDevice::print_description() {
    auto properties =get_physical_properties();

    std::cout << "VK::PhysicalDevice[" << properties->deviceName << "]:\n";
    std::cout << "\tapiVersion: " << VK::VersionCode(properties->apiVersion) << "\n";
    std::cout << "\tdriverVersion: " << VK::VersionCode(properties->driverVersion) << "\n";
    std::cout << "\tvendorID: " << properties->vendorID << "\n";
    std::cout << "\tdeviceID: " << properties->deviceID << "\n";
    std::cout << "\tdeviceType: " << VK::PhysicalDeviceTypeCode(properties->deviceType) << "\n";
}

VkFormat VK::PhysicalDevice::find_supported_format(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                                   VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props {};
        get_format_properties(&props, format);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format");
}

bool VK::PhysicalDevice::has_surface_present_modes(const VK::UnownedSurface &surface) const {
    uint32_t present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_handle, surface.get_handle(), &present_mode_count, nullptr);
    return present_mode_count > 0;
}

std::vector<VkPresentModeKHR> VK::PhysicalDevice::get_surface_present_modes(const VK::UnownedSurface &surface) const {
    std::vector<VkPresentModeKHR> m_present_modes {};
    uint32_t present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_handle, surface.get_handle(), &present_mode_count, nullptr);
    m_present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_handle, surface.get_handle(), &present_mode_count, m_present_modes.data());
    return m_present_modes;
}

VK::SwapchainCapabilities VK::PhysicalDevice::get_surface_capabilities(const VK::UnownedSurface &surface) const {
    VK::SwapchainCapabilities result {};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_handle, surface.get_handle(), &result.get_description());
    return result;
}

bool VK::PhysicalDevice::has_supported_surface_formats(const VK::UnownedSurface &surface) const {
    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_handle, surface.get_handle(), &format_count, nullptr);
    return format_count > 0;
}

std::vector<VkSurfaceFormatKHR> VK::PhysicalDevice::get_supported_surface_formats(const VK::UnownedSurface &surface) const {
    std::vector<VkSurfaceFormatKHR> formats {};
    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_handle, surface.get_handle(), &format_count, nullptr);
    formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_handle, surface.get_handle(), &format_count, formats.data());
    return formats;
}

bool VK::PhysicalDevice::supports_surface_format(const VK::UnownedSurface &surface, VkSurfaceFormatKHR format) const {
    auto formats = get_supported_surface_formats(surface);
    return std::find_if(formats.begin(), formats.end(), [format](VkSurfaceFormatKHR each) -> bool {
        return each.format == format.format && each.colorSpace == format.colorSpace;
    }) != formats.end();
}

bool VK::PhysicalDevice::supports_surface_present_mode(const VK::UnownedSurface &surface, VkPresentModeKHR mode) const {
    auto modes = get_surface_present_modes(surface);
    return std::find(modes.begin(), modes.end(), mode) != modes.end();
}
