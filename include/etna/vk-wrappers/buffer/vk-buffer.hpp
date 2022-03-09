#pragma once

#include <exception>
#include <etna/vk-wrappers/resources/vk-device-resource.hpp>

namespace VK {

using UnownedBuffer = UnownedDeviceResource<VkBuffer>;

class Buffer: public DeviceResource<VkBuffer> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    Buffer(Buffer&& move) noexcept = default;
    Buffer& operator=(Buffer&& move_assign) = default;

    ~Buffer() override { destroy(); }

    void destroy() final {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyBuffer(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }
};

}