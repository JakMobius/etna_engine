//
// Created by Артем on 14.02.2022.
//

#include "vk-image.hpp"
#include "../barriers/vk-image-memory-barrier.hpp"

void VK::Image::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkDestroyImage(this->m_device->get_handle(), this->m_handle, nullptr);
    this->m_handle = nullptr;
}
