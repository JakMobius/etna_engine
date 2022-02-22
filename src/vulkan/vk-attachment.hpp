#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class Attachment {
    VkAttachmentDescription m_description {};

public:
    explicit Attachment(VkFormat format) {
        m_description.format = format;
        m_description.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        m_description.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        m_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        m_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        m_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        m_description.finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        m_description.samples = VK_SAMPLE_COUNT_1_BIT;
    }

    void set_load_store_operations(VkAttachmentLoadOp load_op, VkAttachmentStoreOp store_op) {
        m_description.loadOp = load_op;
        m_description.storeOp = store_op;
    }

    void set_stencil_load_store_operations(VkAttachmentLoadOp load_op, VkAttachmentStoreOp store_op) {
        m_description.stencilLoadOp = load_op;
        m_description.stencilStoreOp = store_op;
    }

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