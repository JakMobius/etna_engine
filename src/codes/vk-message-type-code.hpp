#pragma once

#include <vulkan/vulkan_core.h>
#include <ostream>

class VkMessageTypeCode {
    VkDebugUtilsMessageTypeFlagsEXT m_code;
public:
    explicit VkMessageTypeCode(VkDebugUtilsMessageTypeFlagsEXT code): m_code(code) {}

    static const char* name(VkDebugUtilsMessageTypeFlagsEXT result);

    VkDebugUtilsMessageTypeFlagsEXT get_code() const { return m_code; }
};

std::ostream& operator<<(std::ostream& stream, const VkMessageTypeCode& result);