#pragma once

#include <vulkan/vulkan_core.h>
#include "../vk-device.hpp"
#include "../vk-device-resource.hpp"

namespace VK {

using UnownedPipeline = UnownedDeviceResource<VkPipeline>;

class Pipeline : public DeviceResource<VkPipeline> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    Pipeline(Pipeline &&move) noexcept = default;

    Pipeline &operator=(Pipeline &&move_assign) = default;

    ~Pipeline() override { destroy(); }

    void destroy() {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyPipeline(m_device->get_handle(), m_handle, nullptr);
        m_handle = nullptr;
    }
};
}