#pragma once

#include <vulkan/vulkan_core.h>
#include "../resources/vk-device-resource.hpp"
#include "../image/vk-image.hpp"
#include "../vk-semaphore.hpp"
#include "../vk-fence.hpp"

namespace VK {

class SwapchainBase: public DeviceResourceBase<VkSwapchainKHR> {
public:
    using DeviceResourceBase::DeviceResourceBase;
    SwapchainBase& operator=(SwapchainBase&& move_assign) = default;

    std::vector<UnownedImage> get_swapchain_images();

    VkResult acquire_next_image(uint32_t* image_index, const Semaphore& semaphore = {}, const Fence& fence = {}, uint64_t timeout = UINT64_MAX) {
        return vkAcquireNextImageKHR(m_device->get_handle(), m_handle, timeout, semaphore.get_handle(), fence.get_handle(), image_index);
    }

    ~SwapchainBase() override = default;
};

using UnownedSwapchain = UnownedDeviceResource<VkSwapchainKHR, SwapchainBase>;

class Swapchain: public DeviceResource<VkSwapchainKHR, SwapchainBase> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    Swapchain(Swapchain&& move) noexcept = default;
    Swapchain& operator=(Swapchain&& move_assign) = default;

    ~Swapchain() override { destroy(); }

    void destroy() final {
        if(!this->m_handle || !this->m_device) return;
        vkDestroySwapchainKHR(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }
};

}