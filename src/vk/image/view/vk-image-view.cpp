//
// Created by Артем on 14.02.2022.
//

#include <etna/vk/image/view/vk-image-view.hpp>

void VK::ImageView::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkDestroyImageView(this->m_device->get_handle(), this->m_handle, nullptr);
    this->m_handle = nullptr;
}
