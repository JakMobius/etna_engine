
#pragma once

#include <etna/vk-wrappers/vk-memory.hpp>
#include "vk-buffer.hpp"

namespace VK {

class MemoryBuffer {
protected:
    Memory m_memory;
    Buffer m_buffer;

public:
    explicit MemoryBuffer(Buffer&& buffer): m_memory(buffer.get_device()), m_buffer(std::move(buffer)) {}
    MemoryBuffer(MemoryBuffer&& move) noexcept: m_memory(std::move(move.m_memory)), m_buffer(std::move(move.m_buffer)) {}
    MemoryBuffer& operator=(MemoryBuffer&& move_assign)  noexcept {
        m_memory = std::move(move_assign.m_memory);
        m_buffer = std::move(move_assign.m_buffer);
        return *this;
    }

    MemoryBuffer(const MemoryBuffer& copy) = delete;
    MemoryBuffer& operator=(MemoryBuffer& copy_assign) = delete;

    void create(uint32_t memory_type);

    void destroy();

    Memory& get_memory() { return m_memory; }
    const Memory& get_memory() const { return m_memory; }
    Buffer& get_buffer() { return m_buffer; }
    const Buffer& get_buffer() const { return m_buffer; }
};

}