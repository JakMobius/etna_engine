#pragma once

#include <vector>
#include <etna/volk.hpp>
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

    RenderPassFactory& set_flags(VkRenderPassCreateFlags flags) {
        m_description.flags = flags;
        return *this;
    }

    std::vector<VkAttachmentDescription>& get_attachment_descriptions() { return m_attachment_descriptions; }
    std::vector<VkSubpassDescription>& get_subpass_descriptions() { return m_subpass_descriptions; }
    std::vector<VkSubpassDependency>& get_subpass_dependency_descriptions() { return m_subpass_dependency_descriptions; }

    RenderPass create(Device* device);
};

}