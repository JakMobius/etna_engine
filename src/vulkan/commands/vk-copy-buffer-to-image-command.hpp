#pragma once

namespace VK {
class Buffer;
class Image;
}

#include <vulkan/vulkan_core.h>
#include "vk-command.hpp"
#include "../image/vk-image.hpp"
#include "../buffer/vk-buffer.hpp"
#include "../image/vk-image-subresource-layers.hpp"

namespace VK {

class CopyBufferToImageCommand : public Command {

    VkBufferImageCopy m_region {};
    VkImageLayout m_dst_image_layout = VK_IMAGE_LAYOUT_GENERAL;

    UnownedBuffer m_buffer;
    UnownedImage m_image;

public:
    CopyBufferToImageCommand(const UnownedBuffer& buffer, const UnownedImage& image);

    VkBufferImageCopy& get_region() { return m_region; }
    VkImageLayout get_destination_image_layout() { return m_dst_image_layout; }

    VK::ImageSubresourceLayers& get_image_subresource_layers() {
        return VK::ImageSubresourceLayers::existing(m_region.imageSubresource);
    }

    const VK::ImageSubresourceLayers& get_image_subresource_layers() const {
        return VK::ImageSubresourceLayers::existing(m_region.imageSubresource);
    }

    void set_image_extent(VkExtent3D extent) {
        m_region.imageExtent = extent;
    }

    void set_destination_image_layout(VkImageLayout layout) {
        m_dst_image_layout = layout;
    }

    void set_image_subresource_layers(const VK::ImageSubresourceLayers& layers) {
        m_region.imageSubresource = layers.get_description();
    }

    void write(VK::CommandBuffer *command_buffer) override;

};

}