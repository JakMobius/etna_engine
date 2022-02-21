#pragma once

#include <vulkan/vulkan_core.h>
#include "../vk-device.hpp"

namespace VK {

class Pipeline {
    Device* m_device;
    VkPipeline m_handle;
public:
    Pipeline(Device* device, VkPipeline pipeline): m_device(device), m_handle(pipeline) {}
    Pipeline(const Pipeline& copy) = delete;

    ~Pipeline() {
        destroy();
    }

    void destroy() {
        if(m_handle) {
            vkDestroyPipeline(m_device->get_handle(), m_handle, nullptr);
            m_handle = nullptr;
        }
    }

    VkPipeline get_handle() { return m_handle; }
};

}