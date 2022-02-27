#pragma once

#include <vulkan/vulkan_core.h>
#include "../vk-device.hpp"
#include "../image/view/vk-image-view.hpp"

namespace VK {

using UnownedFramebuffer = UnownedDeviceResource<VkFramebuffer>;

class Framebuffer : public DeviceResource<VkFramebuffer> {
public:

    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    Framebuffer(Framebuffer &&move) noexcept = default;
    Framebuffer &operator=(Framebuffer &&move_assign) = default;

    ~Framebuffer() override { destroy(); }

    void destroy() final {
        if(!m_handle) return;
        vkDestroyFramebuffer(m_device->get_handle(), m_handle, nullptr);
        m_handle = nullptr;
    }

};

}