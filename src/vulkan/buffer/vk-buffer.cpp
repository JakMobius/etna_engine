//
// Created by Артем on 12.02.2022.
//

#include "vk-buffer.hpp"

void VK::Buffer::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkDestroyBuffer(this->m_device->get_handle(), this->m_handle, nullptr);
    this->m_handle = nullptr;
}
