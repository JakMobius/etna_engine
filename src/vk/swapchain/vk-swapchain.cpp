
#include <etna/vk-wrappers/swapchain/vk-swapchain.hpp>

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

void VK::Swapchain::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkDestroySwapchainKHR(this->m_device->get_handle(), this->m_handle, nullptr);
    this->m_handle = nullptr;
}
