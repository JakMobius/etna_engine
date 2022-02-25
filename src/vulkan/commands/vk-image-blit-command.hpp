#pragma once

#include "vk-command.hpp"
#include "../image/vk-image.hpp"

namespace VK {

class ImageBlitCommand : public Command {

    VkImageBlit m_description {};
    Image* m_source = nullptr;
    Image* m_destination = nullptr;
    VkFilter m_filter = VK_FILTER_LINEAR;
    VkImageLayout m_source_layout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageLayout m_destination_layout = VK_IMAGE_LAYOUT_UNDEFINED;

public:
    ImageBlitCommand(Image* source, Image* destination): m_source(source), m_destination(destination) {
        m_description.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        m_description.srcSubresource.mipLevel = 0;
        m_description.srcSubresource.baseArrayLayer = 0;
        m_description.srcSubresource.layerCount = 1;

        m_description.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        m_description.dstSubresource.mipLevel = 0;
        m_description.dstSubresource.baseArrayLayer = 0;
        m_description.dstSubresource.layerCount = 1;
    }

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

    void write(VK::CommandBuffer* command_buffer) override {

        vkCmdBlitImage(command_buffer->get_handle(),
                       m_source->get_handle(), m_source_layout,
                       m_destination->get_handle(), m_destination_layout,
                       1, &m_description,
                       m_filter);
    }


    void setup_mip_offsets(int32_t mip_width, int32_t mip_height) {
        m_description.srcOffsets[0] = { 0, 0, 0 };
        m_description.srcOffsets[1] = { mip_width, mip_height, 1 };

        m_description.dstOffsets[0] = { 0, 0, 0 };
        m_description.dstOffsets[1] = { mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1 };
    }
};

}