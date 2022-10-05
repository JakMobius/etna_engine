#pragma once

#include <etna/volk.hpp>
#include <ostream>

namespace VK {

class PhysicalDeviceTypeCode {
    VkPhysicalDeviceType m_code;
public:
    explicit PhysicalDeviceTypeCode(VkPhysicalDeviceType code):
    m_code(code) {}

    static const char* name(VkPhysicalDeviceType result);

    VkPhysicalDeviceType get_code() const { return m_code; }
};

}

std::ostream& operator<<(std::ostream& stream, const VK::PhysicalDeviceTypeCode& result);