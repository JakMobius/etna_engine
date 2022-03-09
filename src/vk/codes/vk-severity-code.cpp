
#include <etna/vk-wrappers/codes/vk-severity-code.hpp>


const char* VK::SeverityCode::name(VkDebugUtilsMessageSeverityFlagBitsEXT code) {
    switch(code) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:   return "Verbose";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:      return "Info";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:   return "Warning";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:     return "Error";
        default: return "Unknown severity";
    }
}

std::ostream &operator<<(std::ostream &stream, const VK::SeverityCode &code) {
    stream << VK::SeverityCode::name(code.get_code());
    return stream;
}
