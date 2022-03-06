#pragma once

#include "../../../vulkan/commands/vk-copy-buffer-to-image-command.hpp"
#include "../../image.hpp"
#include "../../../vulkan/buffer/vk-buffer.hpp"
#include "../command-image.hpp"

namespace Etna {

class CommandBufferToImageTransfer {

    CommandImage* m_target;
    VK::CopyBufferToImageCommand m_command;

public:
    CommandBufferToImageTransfer(const VK::UnownedBuffer &source, CommandImage* target):
            m_target(target),
            m_command(source, target->get_etna_image()->get_image().unowned_copy()) {
        m_command.set_image_extent(target->get_etna_image()->get_extent());
    }

    VK::ImageSubresourceLayers& get_image_subresource_layers() { return m_command.get_image_subresource_layers(); }
    const VK::ImageSubresourceLayers& get_image_subresource_layers() const { return m_command.get_image_subresource_layers(); }
    void set_image_subresource_layers(const VK::ImageSubresourceLayers& layers) { m_command.set_image_subresource_layers(layers); }

    void perform(VK::CommandBuffer* buffer) {
        m_command.set_destination_image_layout(m_command.get_destination_image_layout());
        m_command.write(buffer);
    }
};

}