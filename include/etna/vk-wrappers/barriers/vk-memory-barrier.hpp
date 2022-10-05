#pragma once

#include <etna/vk-wrappers/image/vk-image.hpp>
#include <etna/vk-wrappers/image/vk-image-subresource-range.hpp>
#include <etna/vk-wrappers/command-buffer.hpp>
#include <etna/vk-wrappers/vk-memory.hpp>

namespace VK {

class MemoryBarrier {

    VkMemoryBarrier m_description {};

public:
    explicit MemoryBarrier();

    MemoryBarrier& set_access_masks(VkAccessFlags src_masks, VkAccessFlags dst_masks) {
        m_description.srcAccessMask = src_masks;
        m_description.dstAccessMask = dst_masks;
        return *this;
    }

    VkMemoryBarrier& get_description() {
        return m_description;
    }

    void write(const UnownedCommandBuffer& command_buffer, VkPipelineStageFlags src_mask, VkPipelineStageFlags dst_mask);
};

}
