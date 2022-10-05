#pragma once

#include <etna/vk-wrappers/image/vk-image.hpp>
#include <etna/vk-wrappers/image/vk-image-subresource-range.hpp>
#include <etna/vk-wrappers/command-buffer.hpp>
#include <etna/vk-wrappers/vk-memory.hpp>

namespace VK {

class BufferMemoryBarrier {

    VkBufferMemoryBarrier m_description {};

public:
    explicit BufferMemoryBarrier(const VK::Buffer& buffer);

    BufferMemoryBarrier& set_access_masks(VkAccessFlags src_masks, VkAccessFlags dst_masks) {
        m_description.srcAccessMask = src_masks;
        m_description.dstAccessMask = dst_masks;
        return *this;
    }

    BufferMemoryBarrier& set_queue_family_indices(uint32_t src_family, uint32_t dst_family) {
        m_description.srcQueueFamilyIndex = src_family;
        m_description.dstQueueFamilyIndex = dst_family;
        return *this;
    }

    BufferMemoryBarrier& set_offset(VkDeviceSize offset) {
        m_description.offset = offset;
        return *this;
    }

    BufferMemoryBarrier& set_size(VkDeviceSize size) {
        m_description.size = size;
        return *this;
    }

    VkBufferMemoryBarrier& get_description() {
        return m_description;
    }

    void write(const UnownedCommandBuffer& command_buffer, VkPipelineStageFlags src_mask, VkPipelineStageFlags dst_mask);
};

}
