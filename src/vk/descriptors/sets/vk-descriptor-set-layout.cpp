
#include <etna/vk-wrappers/descriptors/sets/vk-descriptor-set-layout.hpp>

void VK::DescriptorSetLayout::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkDestroyDescriptorSetLayout(this->m_device->get_handle(), this->m_handle, nullptr);
    this->m_handle = nullptr;
}
