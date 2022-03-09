
#include <etna/vk-wrappers/vk-attachment.hpp>

VK::Attachment::Attachment(VkFormat format) {
    m_description.format = format;
    m_description.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    m_description.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    m_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    m_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    m_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    m_description.finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    m_description.samples = VK_SAMPLE_COUNT_1_BIT;
}

VK::Attachment& VK::Attachment::set_load_store_operations(VkAttachmentLoadOp load_op, VkAttachmentStoreOp store_op) {
    m_description.loadOp = load_op;
    m_description.storeOp = store_op;
    return *this;
}

VK::Attachment& VK::Attachment::set_stencil_load_store_operations(VkAttachmentLoadOp load_op, VkAttachmentStoreOp store_op) {
    m_description.stencilLoadOp = load_op;
    m_description.stencilStoreOp = store_op;
    return *this;
}
