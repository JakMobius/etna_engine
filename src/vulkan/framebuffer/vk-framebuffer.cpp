//
// Created by Артем on 16.02.2022.
//

#include "vk-framebuffer.hpp"

void VK::Framebuffer::destroy() {
    if(!m_handle) return;
    vkDestroyFramebuffer(m_device->get_handle(), m_handle, nullptr);
    m_handle = nullptr;
}
