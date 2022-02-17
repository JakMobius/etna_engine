#pragma once

#include "vk-device.hpp"

namespace VK {

class Shader {

    VkShaderModule m_handle = nullptr;
    Device* m_device;

public:
    Shader(Device* device, const std::string& code): m_device(device) {
        VkShaderModuleCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(device->get_handle(), &createInfo, nullptr, &m_handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module");
        }
    }

    ~Shader() {
        vkDestroyShaderModule(m_device->get_handle(), m_handle, nullptr);
    }

    VkShaderModule get_handle() { return m_handle; }

};

}