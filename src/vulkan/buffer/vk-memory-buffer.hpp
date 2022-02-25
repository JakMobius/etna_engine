
#pragma once

#include "../vk-memory.hpp"
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

    void create(uint32_t memory_type) {
        auto device = m_memory.get_device();

        VkMemoryRequirements mem_requirements {};
        vkGetBufferMemoryRequirements(device->get_handle(), m_buffer.get_handle(), &mem_requirements);

        m_memory.set_size(mem_requirements.size);
        m_memory.set_type(memory_type);
        m_memory.allocate();
        vkBindBufferMemory(device->get_handle(), m_buffer.get_handle(), m_memory.get_handle(), 0);
    }

    void destroy() {
        m_buffer.destroy();
        m_memory.free();
    }

    Memory& get_memory() { return m_memory; }
    Buffer& get_buffer() { return m_buffer; }
};

}