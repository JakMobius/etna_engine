#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class Attachment {
    VkAttachmentDescription m_description {};

public:
    explicit Attachment(VkFormat format);

    void set_load_store_operations(VkAttachmentLoadOp load_op, VkAttachmentStoreOp store_op);

    void set_stencil_load_store_operations(VkAttachmentLoadOp load_op, VkAttachmentStoreOp store_op);

    void set_samples(VkSampleCountFlagBits samples) {
        m_description.samples = samples;
    }

    void set_initial_layout(VkImageLayout initial_layout) {
        m_description.initialLayout = initial_layout;
    }

    void set_final_layout(VkImageLayout final_layout) {
        m_description.finalLayout = final_layout;
    }

    VkAttachmentDescription& get_description() { return m_description; }
};

}