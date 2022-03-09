
#include <etna/vk-wrappers/render-pass/vk-render-pass.hpp>

void VK::RenderPass::destroy() {
    if(!m_handle) return;
    vkDestroyRenderPass(m_device->get_handle(), m_handle, nullptr);
    m_handle = nullptr;
}
