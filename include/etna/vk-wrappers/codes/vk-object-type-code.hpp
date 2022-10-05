#pragma once

#include <etna/volk.hpp>
#include <ostream>

namespace VK {

class ObjectTypeCode {
    VkObjectType m_code;
public:
    explicit ObjectTypeCode(VkObjectType code) : m_code(code) {}

    static const char* name(VkObjectType result);

    VkObjectType get_code() const { return m_code; }
};

}

std::ostream& operator<<(std::ostream& stream, const VK::ObjectTypeCode& result);