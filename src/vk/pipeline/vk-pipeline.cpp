//
// Created by Артем on 21.02.2022.
//

#include "vk-pipeline.hpp"

void VK::Pipeline::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkDestroyPipeline(m_device->get_handle(), m_handle, nullptr);
    m_handle = nullptr;
}
