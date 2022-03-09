
#include <etna/vk-wrappers/sampler/vk-sampler.hpp>

void VK::Sampler::destroy() {
    if(!m_handle) return;
    vkDestroySampler(m_device->get_handle(), m_handle, nullptr);
    m_handle = nullptr;
}
