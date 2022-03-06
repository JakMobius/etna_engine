#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-command.hpp"
#include "../image/vk-image.hpp"

namespace VK {

class ImageBlitCommand : public Command {

    VkImageBlit m_description {};
    UnownedImage m_source {};
    UnownedImage m_destination {};
    VkFilter m_filter = VK_FILTER_LINEAR;
    VkImageLayout m_source_layout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageLayout m_destination_layout = VK_IMAGE_LAYOUT_UNDEFINED;

public:
    ImageBlitCommand(const UnownedImage& source, const UnownedImage& destination);

    void set_aspect_mask(VkImageAspectFlags aspect_mask) {
        m_description.srcSubresource.aspectMask = aspect_mask;
        m_description.dstSubresource.aspectMask = aspect_mask;
    }

    void set_layer_count(int layout_count) {
        m_description.srcSubresource.layerCount = 1;
        m_description.dstSubresource.layerCount = 1;
    }

    void set_src_layer(int layer) { m_description.srcSubresource.baseArrayLayer = layer; }
    void set_dst_layer(int layer) { m_description.dstSubresource.baseArrayLayer = layer; }

    void set_source_mip_level(uint32_t level) { m_description.srcSubresource.mipLevel = level; }
    void set_destination_mip_level(uint32_t level) { m_description.dstSubresource.mipLevel = level; }

    void set_filter(VkFilter filter) { m_filter = filter; }
    void set_source_layout(VkImageLayout source_layout) { m_source_layout = source_layout; }
    void set_destination_layout(VkImageLayout destination_layout) { m_destination_layout = destination_layout; }

    void write(VK::CommandBuffer* command_buffer) override;

    VkOffset3D* get_src_offsets() { return m_description.srcOffsets; }
    VkOffset3D* get_dst_offsets() { return m_description.dstOffsets; }
};

}