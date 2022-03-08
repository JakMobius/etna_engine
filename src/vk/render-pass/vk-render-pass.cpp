//
// Created by Артем on 26.02.2022.
//

#include <etna/vk/render-pass/vk-render-pass.hpp>

void VK::RenderPass::destroy() {
    if(!m_handle) return;
    vkDestroyRenderPass(m_device->get_handle(), m_handle, nullptr);
    m_handle = nullptr;
}
