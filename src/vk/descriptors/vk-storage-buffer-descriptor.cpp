
#include <etna/vk-wrappers/descriptors/vk-storage-buffer-descriptor.hpp>

VK::StorageBufferDescriptor::StorageBufferDescriptor(const VK::Buffer &buffer, VkDeviceSize offset, VkDeviceSize size) : Descriptor() {
    m_description.dstArrayElement = 0;
    m_description.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    m_description.descriptorCount = 1;
    m_description.pBufferInfo = &m_buffer_info;

    m_buffer_info.buffer = buffer.get_handle();
    m_buffer_info.offset = offset;
    m_buffer_info.range = size;
}
