
#include <etna/vk-wrappers/vk-memory.hpp>

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

VK::Memory &VK::Memory::operator=(VK::Memory &&move_assign) noexcept {
    if(this == &move_assign) return *this;
    free();
    m_device = move_assign.m_device;
    m_handle = move_assign.m_handle;
    m_size = move_assign.m_size;
    m_memory_type = move_assign.m_memory_type;
    move_assign.m_handle = nullptr;
    return *this;
}

void VK::Memory::allocate() {
    if(!m_device) {
        throw std::runtime_error("cannot to allocate memory without specifying a device");
    }

    if(m_size == 0) {
        throw std::runtime_error("attempt to allocate zero-length VK::Memory");
    }

    if(m_handle) free();

    VkMemoryAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = m_size;
    alloc_info.memoryTypeIndex = m_memory_type;

    if (vkAllocateMemory(m_device->get_handle(), &alloc_info, nullptr, &m_handle) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }
}

void* VK::Memory::map() {
    void* mapped_ptr = nullptr;
    vkMapMemory(m_device->get_handle(), m_handle, 0, m_size, 0, &mapped_ptr);
    return mapped_ptr;
}

void VK::Memory::unmap() {
    vkUnmapMemory(m_device->get_handle(), m_handle);
}

void VK::Memory::free() {
    if(!m_handle) return;

    vkFreeMemory(m_device->get_handle(), m_handle, nullptr);
    m_handle = nullptr;
}

void VK::Memory::set_data(const void* data, size_t size, size_t offset) {
    void* mapped_ptr = map();
    memcpy((char*)mapped_ptr + offset, data, size);
    flush(offset, size);
    unmap();
}

void VK::Memory::get_data(void* data, size_t size, size_t offset) {
    void* mapped_ptr = map();
    memcpy(data, (char*)mapped_ptr + offset, size);
    unmap();
}

VK::Memory::Memory(VK::Memory &&move) noexcept: m_handle(move.m_handle), m_size(move.m_size), m_device(move.m_device), m_memory_type(move.m_memory_type) {
    move.m_handle = nullptr;
}
