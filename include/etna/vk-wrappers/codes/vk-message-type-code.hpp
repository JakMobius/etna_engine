
#pragma once

#include <etna/volk.hpp>
#include <ostream>

namespace VK {

class MessageTypeCode {
    VkDebugUtilsMessageTypeFlagsEXT m_code;
public:
    explicit MessageTypeCode(VkDebugUtilsMessageTypeFlagsEXT code) : m_code(code) {}

    static const char* name(VkDebugUtilsMessageTypeFlagsEXT result);

    VkDebugUtilsMessageTypeFlagsEXT get_code() const { return m_code; }
};

}

std::ostream& operator<<(std::ostream& stream, const VK::MessageTypeCode& result);