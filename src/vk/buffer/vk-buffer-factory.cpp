
#include <etna/vk-wrappers/buffer/vk-buffer-factory.hpp>

VkBuffer VK::BufferFactory::create_raw_buffer(VK::Device* device) {

    m_create_info.pQueueFamilyIndices = m_queue_families.data();
    m_create_info.queueFamilyIndexCount = m_queue_families.size();

    VkBuffer buffer = nullptr;

    if (vkCreateBuffer(device->get_handle(), &m_create_info, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer");
    }

    return buffer;
}

VK::MemoryBuffer VK::BufferFactory::create_memory_buffer(VK::Device* device) {
    auto physical_device = device->get_physical_device();

    VkBuffer raw_buffer = create_raw_buffer(device);
    VK::Buffer buffer { device, raw_buffer };

    VkMemoryRequirements mem_requirements {};
    vkGetBufferMemoryRequirements(device->get_handle(), buffer.get_handle(), &mem_requirements);
    auto memory_type = physical_device->get_suitable_memory_type(mem_requirements.memoryTypeBits, m_memory_properties);

    auto result = MemoryBuffer { std::move(buffer) };
    result.create(memory_type);
    return result;
}
