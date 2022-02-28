//
// Created by Артем on 28.02.2022.
//

#include "vk-surface.hpp"

void VK::Surface::destroy() {
    if(!this->m_handle) return;
    vkDestroySurfaceKHR(this->m_instance->get_handle(), this->m_handle, nullptr);
    this->m_handle = nullptr;
}
