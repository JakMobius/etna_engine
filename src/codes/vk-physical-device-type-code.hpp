#pragma once

#include <vulkan/vulkan_core.h>
#include <ostream>

class VkPhysicalDeviceTypeCode {
    VkPhysicalDeviceType m_code;
public:
    explicit VkPhysicalDeviceTypeCode(VkPhysicalDeviceType code): m_code(code) {}

    static const char* name(VkPhysicalDeviceType result);

    VkPhysicalDeviceType get_code() const { return m_code; }
};

std::ostream& operator<<(std::ostream& stream, const VkPhysicalDeviceTypeCode& result);