
#include <etna/vk-wrappers/image/vk-memory-image.hpp>

VK::MemoryImage &VK::MemoryImage::operator=(VK::MemoryImage &&move_assign) noexcept {
    m_memory = std::move(move_assign.m_memory);
    m_image = std::move(move_assign.m_image);
    return *this;
}

void VK::MemoryImage::create(VkMemoryPropertyFlags memory_properties) {
    auto device = m_image.get_device();

    VkMemoryRequirements mem_requirements {};
    vkGetImageMemoryRequirements(device->get_handle(), m_image.get_handle(), &mem_requirements);

    auto mem_type = device->get_physical_device()->get_suitable_memory_type(mem_requirements.memoryTypeBits, memory_properties);

    m_memory.set_device(device);
    m_memory.set_size(mem_requirements.size);
    m_memory.set_type(mem_type);
    m_memory.allocate();

    vkBindImageMemory(device->get_handle(), m_image.get_handle(), m_memory.get_handle(), 0);
}

void VK::MemoryImage::destroy() {
    m_image.destroy();
    m_memory.free();
}
