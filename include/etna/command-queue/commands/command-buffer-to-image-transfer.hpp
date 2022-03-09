#pragma once

#include <etna/vk-wrappers/commands/vk-copy-buffer-to-image-command.hpp>
#include <etna/image.hpp>
#include <etna/vk-wrappers/buffer/vk-buffer.hpp>
#include "../command-image.hpp"

namespace Etna {

class CommandBufferToImageTransfer {

    CommandImage* m_target;
    VK::CopyBufferToImageCommand m_command;

public:
    CommandBufferToImageTransfer(const VK::UnownedBuffer &source, CommandImage* target):
            m_target(target),
            m_command(source, target->get_etna_image()->get_image()) {
        m_command.set_image_extent(target->get_etna_image()->get_extent());
    }

    VK::ImageSubresourceLayers& get_image_subresource_layers() { return m_command.get_image_subresource_layers(); }
    const VK::ImageSubresourceLayers& get_image_subresource_layers() const { return m_command.get_image_subresource_layers(); }

    CommandBufferToImageTransfer &set_image_subresource_layers(const VK::ImageSubresourceLayers &layers) {
        m_command.set_image_subresource_layers(layers);
        return *this;
    }

    void perform(Etna::CommandQueue* buffer) {
        m_command.set_destination_image_layout(m_target->get_state().m_layout);
        m_command.write(buffer->get_command_buffer());
    }
};

}