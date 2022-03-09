
#include <etna/vk-wrappers/vk-fence.hpp>

VK::Fence VK::Fence::create(VK::Device* device, VkFenceCreateFlags flags) {
    VkFenceCreateInfo fence_info {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = flags;

    VkFence fence = nullptr;
    if(vkCreateFence(device->get_handle(), &fence_info, nullptr, &fence)) {
        throw std::runtime_error("failed to create a fence");
    }

    return {device, fence};
}

void VK::Fence::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkDestroyFence(this->m_device->get_handle(), this->m_handle, nullptr);
    this->m_handle = nullptr;
}
