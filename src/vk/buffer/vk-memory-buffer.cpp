
#include <etna/vk-wrappers/buffer/vk-memory-buffer.hpp>

void VK::MemoryBuffer::create(uint32_t memory_type) {
    auto device = m_memory.get_device();

    VkMemoryRequirements mem_requirements {};
    vkGetBufferMemoryRequirements(device->get_handle(), m_buffer.get_handle(), &mem_requirements);

    m_memory.set_size(mem_requirements.size);
    m_memory.set_type(memory_type);
    m_memory.allocate();
    vkBindBufferMemory(device->get_handle(), m_buffer.get_handle(), m_memory.get_handle(), 0);
}

void VK::MemoryBuffer::destroy() {
    m_buffer.destroy();
    m_memory.free();
}
