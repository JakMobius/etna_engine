
#pragma once

#include "vk-memory.hpp"
#include "vk-buffer.hpp"

namespace VK {

class MemoryBuffer {
protected:
    Memory m_memory;
    Buffer m_buffer;

public:
    explicit MemoryBuffer(Device* device): m_memory(device), m_buffer(&m_memory) {

    }

    void destroy() {
        m_buffer.destroy();
        m_memory.free();
    }

    Memory& get_memory() { return m_memory; }
    Buffer& get_buffer() { return m_buffer; }
};

}