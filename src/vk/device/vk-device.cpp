
#include <etna/vk-wrappers/device/vk-device.hpp>
#include <etna/vk-wrappers/queue/vk-queue.hpp>

VK::Device &VK::Device::operator=(VK::Device &&move_assign) noexcept {
    if(this == &move_assign) return *this;
    destroy();
    m_physical_device = move_assign.m_physical_device;
    m_handle = move_assign.m_handle;
    move_assign.m_handle = nullptr;
    return *this;
}

VK::Queue VK::Device::get_queue(uint32_t queue_family, uint32_t index) {
    VkQueue result = nullptr;
    vkGetDeviceQueue(m_handle, queue_family, index, &result);
    return VK::Queue { result };
}

void VK::Device::destroy() {
    if(!m_handle) return;
    vkDestroyDevice(m_handle, nullptr);
    m_handle = nullptr;
}
