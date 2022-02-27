//
// Created by Артем on 27.02.2022.
//

#include "vk-swapchain.hpp"

std::vector<VK::UnownedImage> VK::SwapchainBase::get_swapchain_images() {
    uint32_t image_count = 0;
    std::vector<VkImage> images {};
    vkGetSwapchainImagesKHR(m_device->get_handle(), m_handle, &image_count, nullptr);
    images.resize(image_count);
    vkGetSwapchainImagesKHR(m_device->get_handle(), m_handle, &image_count, images.data());

    std::vector<VK::UnownedImage> result;
    result.reserve(image_count);

    for(int i = 0; i < image_count; i++) result.emplace_back(m_device, images[i]);

    return result;
}