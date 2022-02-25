#pragma once

#include <vulkan/vulkan_core.h>
#include "../vk-device-resource.hpp"
#include "../vk-memory.hpp"
#include "../vk-command-buffer.hpp"

namespace VK {

class UnownedImage: public UnownedDeviceResource<VkImage> {
public:
    using UnownedDeviceResource<VkImage>::UnownedDeviceResource;
    using UnownedDeviceResource<VkImage>::operator=;
};

class Image: public DeviceResource<VkImage> {
public:
    using DeviceResource<VkImage>::DeviceResource;
    using DeviceResource<VkImage>::operator=;

    Image(Image&& move) noexcept = default;
    Image& operator=(Image&& move_assign) = default;

    ~Image() override { destroy(); }

    void destroy() {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyImage(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }

    UnownedImage unowned_copy() {
        return UnownedImage { this->m_device, this->m_handle };
    }
};

}