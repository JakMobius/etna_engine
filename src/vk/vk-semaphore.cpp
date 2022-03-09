
#include <etna/vk-wrappers/vk-semaphore.hpp>

VK::Semaphore VK::Semaphore::create(VK::Device* device, VkSemaphoreCreateFlags flags) {
    VkSemaphoreCreateInfo semaphore_info {};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_info.flags = flags;

    VkSemaphore semaphore = nullptr;
    if(vkCreateSemaphore(device->get_handle(), &semaphore_info, nullptr, &semaphore)) {
        throw std::runtime_error("failed to create a semaphore");
    }

    return {device, semaphore};
}

void VK::Semaphore::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkDestroySemaphore(this->m_device->get_handle(), this->m_handle, nullptr);
    this->m_handle = nullptr;
}
