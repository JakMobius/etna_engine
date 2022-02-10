#pragma once

#include <vulkan/vulkan_core.h>
#include <ostream>

class VkSeverityCode {
    VkDebugUtilsMessageSeverityFlagBitsEXT m_code;
public:
    explicit VkSeverityCode(VkDebugUtilsMessageSeverityFlagBitsEXT code): m_code(code) {};

    static const char* name(VkDebugUtilsMessageSeverityFlagBitsEXT result);

    VkDebugUtilsMessageSeverityFlagBitsEXT get_code() const { return m_code; }
};

std::ostream& operator<<(std::ostream& stream, const VkSeverityCode& result);