#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-device.hpp"

namespace VK {

class Sampler {

    VkSampler m_handle;
    VK::Device* m_device;

public:
    Sampler(VkSampler sampler, VK::Device* device): m_handle(sampler), m_device(device) {

    }

    ~Sampler() {
        destroy();
    }

    VkSampler get_handle() { return m_handle; }

    void destroy() {
        if(m_handle) {
            vkDestroySampler(m_device->get_handle(), m_handle, nullptr);
            m_handle = nullptr;
        }
    }
};

}