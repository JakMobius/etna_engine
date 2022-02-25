#pragma once

#include <exception>
#include "../vk-memory.hpp"
#include "../vk-command-buffer.hpp"
#include "../vk-device-resource.hpp"

namespace VK {

class UnownedBuffer: public UnownedDeviceResource<VkBuffer> {
public:
    using UnownedDeviceResource<VkBuffer>::UnownedDeviceResource;
    using UnownedDeviceResource<VkBuffer>::operator=;
};

class Buffer: public DeviceResource<VkBuffer> {
public:
    using DeviceResource<VkBuffer>::DeviceResource;
    using DeviceResource<VkBuffer>::operator=;

    Buffer(Buffer&& move) noexcept = default;
    Buffer& operator=(Buffer&& move_assign) = default;

    ~Buffer() override { destroy(); }

    void destroy() {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyBuffer(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }

    UnownedBuffer unowned_copy() {
        return UnownedBuffer { this->m_device, this->m_handle };
    }
};

}