#pragma once

#include <cstdint>
#include <ostream>

class VkVersionCode {

    uint32_t m_code;

    static uint32_t mask(uint32_t code, uint32_t bit_high, uint32_t bit_low) {
        uint32_t bits = bit_high - bit_low + 1;
        return (code >> bit_low) & ((1 << bits) - 1);
    }

public:
    explicit VkVersionCode(uint32_t code): m_code(code) {}


    uint32_t get_variant()       const { return mask(m_code, 31, 29); }
    uint32_t get_major_version() const { return mask(m_code, 28, 22); }
    uint32_t get_minor_version() const { return mask(m_code, 21, 12); }
    uint32_t get_patch()         const { return mask(m_code, 11, 0); }
    uint32_t get_code()          const { return m_code; }
};

std::ostream& operator<<(std::ostream& stream, const VkVersionCode& version);
