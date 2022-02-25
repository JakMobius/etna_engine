#pragma once

#include "vk-command.hpp"
#include "../buffer/vk-buffer.hpp"
#include "../image/vk-image.hpp"

namespace VK {

class CopyBufferToImageCommand : public Command {

    VkBufferImageCopy m_region {};
    VkImageLayout m_dst_image_layout = VK_IMAGE_LAYOUT_GENERAL;

    Buffer* m_buffer;
    Image* m_image;

public:
    CopyBufferToImageCommand(Buffer* buffer, Image* image): m_buffer(buffer), m_image(image) {

        m_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        m_region.imageSubresource.layerCount = 1;
        m_region.imageExtent = { 0, 0, 1 };
    }

    VkBufferImageCopy& get_region() { return m_region; }
    VkImageLayout get_destination_image_layout() { return m_dst_image_layout; }

    VkImageSubresourceLayers& get_image_subresource() {
        return m_region.imageSubresource;
    }

    void set_image_extent(VkExtent3D extent) {
        m_region.imageExtent = extent;
    }

    void set_destination_image_layout(VkImageLayout layout) { m_dst_image_layout = layout; }

    void write(VK::CommandBuffer *command_buffer) override {
        vkCmdCopyBufferToImage(
            command_buffer->get_handle(),
            m_buffer->get_handle(),
            m_image->get_handle(),
            m_dst_image_layout,
            1,
            &m_region
        );
    }

};

}