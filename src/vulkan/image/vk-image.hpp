#pragma once

#include <vulkan/vulkan_core.h>
#include "../resources/vk-device-resource.hpp"
#include "../vk-memory.hpp"
#include "../command-buffer/vk-command-buffer.hpp"

namespace VK {

using ImageBase = DeviceResourceBase<VkImage>;
using UnownedImage = UnownedDeviceResource<VkImage>;

class Image: public DeviceResource<VkImage> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    Image(Image&& move) noexcept = default;
    Image& operator=(Image&& move_assign) = default;

    ~Image() override { destroy(); }

    void destroy() final {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyImage(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }
};

}