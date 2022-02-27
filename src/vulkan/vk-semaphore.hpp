#pragma once

#include <vulkan/vulkan_core.h>
#include "resources/vk-device-resource.hpp"

namespace VK {

class SemaphoreBase: public DeviceResourceBase<VkSemaphore> {
public:
    using DeviceResourceBase::DeviceResourceBase;
    SemaphoreBase& operator=(SemaphoreBase&& move_assign) = default;

    ~SemaphoreBase() override = default;
};

using UnownedSemaphore = UnownedDeviceResource<VkSemaphore, SemaphoreBase>;

class Semaphore: public DeviceResource<VkSemaphore, SemaphoreBase> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    Semaphore(Semaphore&& move) noexcept = default;
    Semaphore& operator=(Semaphore&& move_assign) = default;

    static Semaphore create(VK::Device* device, VkSemaphoreCreateFlags flags = 0) {
        VkSemaphoreCreateInfo semaphore_info {};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphore_info.flags = flags;

        VkSemaphore semaphore = nullptr;
        if(vkCreateSemaphore(device->get_handle(), &semaphore_info, nullptr, &semaphore)) {
            throw std::runtime_error("failed to create a semaphore");
        }

        return {device, semaphore};
    }

    ~Semaphore() override { destroy(); }

    void destroy() final {
        if(!this->m_handle || !this->m_device) return;
        vkDestroySemaphore(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }
};

}