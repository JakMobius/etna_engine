#pragma once

#include <etna/volk.hpp>
#include <ostream>

namespace VK {

class SeverityCode {
    VkDebugUtilsMessageSeverityFlagBitsEXT m_code;
public:
    explicit SeverityCode(VkDebugUtilsMessageSeverityFlagBitsEXT code) : m_code(code) {};

    static const char* name(VkDebugUtilsMessageSeverityFlagBitsEXT result);

    VkDebugUtilsMessageSeverityFlagBitsEXT get_code() const { return m_code; }
};

}

std::ostream& operator<<(std::ostream& stream, const VK::SeverityCode& result);