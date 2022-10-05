#pragma once

#include <etna/volk.hpp>

namespace VK {

class Attachment {
    VkAttachmentDescription m_description {};

public:
    explicit Attachment(VkFormat format);

    Attachment& set_load_store_operations(VkAttachmentLoadOp load_op, VkAttachmentStoreOp store_op);

    Attachment& set_stencil_load_store_operations(VkAttachmentLoadOp load_op, VkAttachmentStoreOp store_op);

    Attachment& set_samples(VkSampleCountFlagBits samples) {
        m_description.samples = samples;
        return *this;
    }

    Attachment& set_initial_layout(VkImageLayout initial_layout) {
        m_description.initialLayout = initial_layout;
        return *this;
    }

    Attachment& set_final_layout(VkImageLayout final_layout) {
        m_description.finalLayout = final_layout;
        return *this;
    }

    VkAttachmentDescription& get_description() { return m_description; }
};

}