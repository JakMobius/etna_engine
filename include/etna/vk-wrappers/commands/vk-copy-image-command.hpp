#pragma once

#include "../image/vk-image.hpp"
#include "../image/vk-image-subresource-layers.hpp"
#include "vk-command.hpp"

namespace VK {

class CopyImageCommand : public Command {

    VkImageLayout m_source_layout = VK_IMAGE_LAYOUT_GENERAL;
    VkImageLayout m_destination_layout = VK_IMAGE_LAYOUT_GENERAL;
    VkImageCopy m_region {};

    UnownedImage m_source;
    UnownedImage m_destination;
public:
    CopyImageCommand(const UnownedImage& source, const UnownedImage& destination): m_source(source), m_destination(destination) {

    }

    ~CopyImageCommand() override {};

    CopyImageCommand& set_source_layout(VkImageLayout source_layout) {
        m_source_layout = source_layout;
        return *this;
    }

    CopyImageCommand& set_destination_layout(VkImageLayout destination_layout) {
        m_destination_layout = destination_layout;
        return *this;
    }

    CopyImageCommand& set_src_subresource_layers(const ImageSubresourceLayers& layers) {
        ImageSubresourceLayers::existing(m_region.srcSubresource) = layers;
        return *this;
    }

    CopyImageCommand& set_dst_subresource_layers(const ImageSubresourceLayers& layers) {
        ImageSubresourceLayers::existing(m_region.srcSubresource) = layers;
        return *this;
    }

    CopyImageCommand& set_src_offset(VkOffset3D offset) {
        m_region.srcOffset = offset;
        return *this;
    }

    CopyImageCommand& set_dst_offset(VkOffset3D offset) {
        m_region.dstOffset = offset;
        return *this;
    }

    CopyImageCommand& set_extent(VkExtent3D extent) {
        m_region.extent = extent;
        return *this;
    }

    ImageSubresourceLayers& get_src_subresource_layers() {
        return ImageSubresourceLayers::existing(m_region.srcSubresource);
    }

    ImageSubresourceLayers& get_dst_subresource_layers() {
        return ImageSubresourceLayers::existing(m_region.srcSubresource);
    }

    void write(const UnownedCommandBuffer& command_buffer) override;


};


}