//
// Created by Артем on 12.02.2022.
//

#include "vk-memory.hpp"

VK::MemoryReference VK::Memory::create_reference() {
    return MemoryReference { this };
}

VK::MemoryReference::MemoryReference(VK::Memory* memory) {
    m_memory = memory;
    if(m_memory) m_memory->add_reference();
}

VK::MemoryReference::~MemoryReference() {
    m_memory->remove_reference();
}
