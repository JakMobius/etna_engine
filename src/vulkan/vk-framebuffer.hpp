#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-device.hpp"
#include "image/vk-image-view.hpp"

namespace VK {

class Framebuffer {

    Device* m_device;
    VkRenderPass m_render_pass;
    std::vector<ImageView*> m_attachments {};
    VkExtent2D m_size {};
    uint32_t m_layers = 1;

    VkFramebuffer m_handle = nullptr;

public:

    Framebuffer(Device* device, VkRenderPass render_pass): m_device(device), m_render_pass(render_pass) {

    }

    ~Framebuffer() {
        destroy();
    }

    void create() {
        std::vector<VkImageView> image_views {};
        image_views.reserve(m_attachments.size());

        for(auto attachment : m_attachments) {
            image_views.push_back(attachment->get_handle());
        }

        VkFramebufferCreateInfo framebuffer_info {};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = m_render_pass;
        framebuffer_info.attachmentCount = m_attachments.size(),
        framebuffer_info.pAttachments = image_views.data();
        framebuffer_info.width = m_size.width;
        framebuffer_info.height = m_size.height;
        framebuffer_info.layers = m_layers;

        if (vkCreateFramebuffer(m_device->get_handle(), &framebuffer_info, nullptr, &m_handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer");
        }
    }

    void destroy() {
        if(m_handle) {
            vkDestroyFramebuffer(m_device->get_handle(), m_handle, nullptr);
            m_handle = nullptr;
        }
    }

    void set_size(const VkExtent2D& size) {
        m_size = size;
    }

    void set_layers(uint32_t layers) {
        m_layers = layers;
    }

    std::vector<ImageView*>& get_attachments() { return m_attachments; }
    VkFramebuffer get_handle() { return m_handle; }
    uint32_t get_layers() { return m_layers; }
    VkExtent2D get_size() { return m_size; }

};

}