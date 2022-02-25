#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "../vk-device.hpp"
#include "vk-render-pass.hpp"

namespace VK {

class RenderPassFactory {
    VkRenderPassCreateInfo m_description {};

    std::vector<VkAttachmentDescription> m_attachment_descriptions {};
    std::vector<VkSubpassDescription> m_subpass_descriptions {};
    std::vector<VkSubpassDependency> m_subpass_dependency_descriptions {};
public:
    RenderPassFactory() {
        m_description.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    }

    void set_flags(VkRenderPassCreateFlags flags) {
        m_description.flags = flags;
    }

    std::vector<VkAttachmentDescription>& get_attachment_descriptions() { return m_attachment_descriptions; }
    std::vector<VkSubpassDescription>& get_subpass_descriptions() { return m_subpass_descriptions; }
    std::vector<VkSubpassDependency>& get_subpass_dependency_descriptions() { return m_subpass_dependency_descriptions; }

    RenderPass create(Device* device) {
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
};

}