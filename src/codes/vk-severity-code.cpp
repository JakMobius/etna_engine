//
// Created by Артем on 09.02.2022.
//

#include "vk-severity-code.hpp"



std::ostream &operator<<(std::ostream &stream, const VkSeverityCode &code) {
    stream << VkSeverityCode::name(code.get_code());
    return stream;
}

const char* VkSeverityCode::name(VkDebugUtilsMessageSeverityFlagBitsEXT code) {
    switch(code) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:   return "Verbose";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:      return "Info";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:   return "Warning";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:     return "Error";
        default: return "Unknown severity";
    }
}
