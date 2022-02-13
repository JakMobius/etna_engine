//
// Created by Артем on 12.02.2022.
//

#include "vk-memory.hpp"

VK::MemoryReference VK::Memory::create_reference() {
    add_reference();
    return MemoryReference { this };
}

void VK::Memory::flush() {
    flush(0, m_size);
}

void VK::Memory::flush(uint32_t offset, uint32_t size) {
    VkMappedMemoryRange flush_range = {
        VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
        nullptr,
        m_handle,
        offset,
        size
    };

    vkFlushMappedMemoryRanges(m_device->get_handle(), 1, &flush_range);
}

VK::MemoryReference::MemoryReference(VK::Memory* memory) {
    m_memory = memory;
    if(m_memory) m_memory->add_reference();
}

VK::MemoryReference::~MemoryReference() {
    m_memory->remove_reference();
}
