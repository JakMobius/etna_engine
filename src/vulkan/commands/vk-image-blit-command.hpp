#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-command.hpp"
#include "../image/vk-image.hpp"
#include "../image/vk-image-subresource-layers.hpp"

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

    ImageSubresourceLayers& get_source_subresource_layers() {
        return ImageSubresourceLayers::existing(m_description.srcSubresource);
    }

    ImageSubresourceLayers& get_destination_subresource_layers() {
        return ImageSubresourceLayers::existing(m_description.dstSubresource);
    }

    const ImageSubresourceLayers& get_source_subresource_layers() const {
        return ImageSubresourceLayers::existing(m_description.srcSubresource);
    }

    const ImageSubresourceLayers& get_destination_subresource_layers() const {
        return ImageSubresourceLayers::existing(m_description.dstSubresource);
    }

    void set_filter(VkFilter filter) { m_filter = filter; }
    void set_source_layout(VkImageLayout source_layout) { m_source_layout = source_layout; }
    void set_destination_layout(VkImageLayout destination_layout) { m_destination_layout = destination_layout; }

    void write(const UnownedCommandBuffer& command_buffer) override;

    VkOffset3D* get_src_offsets() { return m_description.srcOffsets; }
    VkOffset3D* get_dst_offsets() { return m_description.dstOffsets; }
};

}