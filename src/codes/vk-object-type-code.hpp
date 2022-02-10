#pragma once

#include <vulkan/vulkan_core.h>
#include <ostream>

class VkObjectTypeCode {
    VkObjectType m_code;
public:
    explicit VkObjectTypeCode(VkObjectType code): m_code(code) {}

    static const char* name(VkObjectType result);

    VkObjectType get_code() const { return m_code; }
};

std::ostream& operator<<(std::ostream& stream, const VkObjectTypeCode& result);