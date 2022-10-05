#pragma once

#include <cstdint>
#include <ostream>
#include <etna/volk.hpp>

namespace VK {
class VersionCode {

    uint32_t m_code;

public:
    explicit VersionCode(uint32_t code) : m_code(code) {}
    VersionCode(uint32_t variant, uint32_t major, uint32_t minor, uint32_t patch): m_code(0) {
        m_code = VK_MAKE_API_VERSION(variant, major, minor, patch);
    }
    VersionCode(uint32_t major, uint32_t minor, uint32_t patch): m_code(0) {
        m_code = VK_MAKE_API_VERSION(0, major, minor, patch);
    }


    uint32_t get_variant()       const { return VK_API_VERSION_VARIANT(m_code); }
    uint32_t get_major_version() const { return VK_API_VERSION_MAJOR(m_code); }
    uint32_t get_minor_version() const { return VK_API_VERSION_MINOR(m_code); }
    uint32_t get_patch()         const { return VK_API_VERSION_PATCH(m_code); }

    uint32_t get_code() const { return m_code; }
};

}
std::ostream& operator<<(std::ostream& stream, const VK::VersionCode& version);
