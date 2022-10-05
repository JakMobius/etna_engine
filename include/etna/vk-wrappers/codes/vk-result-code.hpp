
#pragma once

#include <etna/volk.hpp>
#include <ostream>

namespace VK {

class ResultCode {
    VkResult m_code;
public:
    explicit ResultCode(VkResult code):
    m_code(code) {}

    static const char* name(VkResult result);

    VkResult get_code() const { return m_code; }
};

}

std::ostream& operator<<(std::ostream& stream, const VK::ResultCode& result);