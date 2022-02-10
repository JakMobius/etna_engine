#pragma once

#include <vulkan/vulkan_core.h>
#include <ostream>

class VkResultCode {
    VkResult m_code;
public:
    explicit VkResultCode(VkResult code): m_code(code) {}

    static const char* name(VkResult result);

    VkResult get_code() const { return m_code; }
};

std::ostream& operator<<(std::ostream& stream, const VkResultCode& result);