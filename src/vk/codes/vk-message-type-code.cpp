
#include <etna/vk-wrappers/codes/vk-message-type-code.hpp>

const char* VK::MessageTypeCode::name(VkDebugUtilsMessageTypeFlagsEXT code) {
    switch(code) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:       return "General";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:    return "Validation";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:   return "Perfomance";
        default: return "Unknown message type";
    }
}

std::ostream &operator<<(std::ostream &stream, const VK::MessageTypeCode &code) {
    stream << VK::MessageTypeCode::name(code.get_code());
    return stream;
}
