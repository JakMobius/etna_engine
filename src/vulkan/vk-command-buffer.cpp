//
// Created by Артем on 13.02.2022.
//

#include "vk-command-buffer.hpp"

VK::CommandBuffer::~CommandBuffer() {
    if(m_handle) {
        auto device = m_pool->get_surface_context()->get_device();
        vkFreeCommandBuffers(device->get_handle(), m_pool->get_handle(), 1, &m_handle);
        m_handle = nullptr;
    }
}
