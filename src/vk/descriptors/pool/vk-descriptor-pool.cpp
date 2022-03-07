//
// Created by Артем on 26.02.2022.
//

#include "vk-descriptor-pool.hpp"

void VK::DescriptorPool::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkDestroyDescriptorPool(this->m_device->get_handle(), this->m_handle, nullptr);
    this->m_handle = nullptr;
}
