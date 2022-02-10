//
// Created by Артем on 09.02.2022.
//

#include <vulkan/vulkan_core.h>
#include "vk-physical-device-type-code.hpp"

const char* VkPhysicalDeviceTypeCode::name(VkPhysicalDeviceType code) {
    switch(code) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER: return "OTHER";
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "INTEGRATED_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "DISCRETE_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "VIRTUAL_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_CPU: return "CPU";
        case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM: return "MAX_ENUM";
    }
}

std::ostream &operator<<(std::ostream &stream, const VkPhysicalDeviceTypeCode &result) {
    stream << VkPhysicalDeviceTypeCode::name(result.get_code());
    return stream;
}
