//
// Created by Артем on 23.02.2022.
//

#include "vk-sampler.hpp"

void VK::Sampler::destroy() {
    if(!m_handle) return;
    vkDestroySampler(m_device->get_handle(), m_handle, nullptr);
    m_handle = nullptr;
}
