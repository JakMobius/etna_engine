
#include <etna/vk-wrappers/render-pass/vk-render-pass-factory.hpp>

VK::RenderPass VK::RenderPassFactory::create(VK::Device* device) {
    m_description.attachmentCount = m_attachment_descriptions.size();
    m_description.pAttachments = m_attachment_descriptions.data();
    m_description.subpassCount = m_subpass_descriptions.size();
    m_description.pSubpasses = m_subpass_descriptions.data();
    m_description.dependencyCount = m_subpass_dependency_descriptions.size();
    m_description.pDependencies = m_subpass_dependency_descriptions.data();

    VkRenderPass render_pass = nullptr;
    if (vkCreateRenderPass(device->get_handle(), &m_description, nullptr, &render_pass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass");
    }
    return { device, render_pass };
}
