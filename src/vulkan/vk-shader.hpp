#pragma once

#include <fstream>
#include <sstream>
#include <vulkan/vulkan_core.h>
#include "vk-device.hpp"
#include "vk-device-resource.hpp"

namespace VK {

using UnownedShaderModule = UnownedDeviceResource<VkShaderModule>;

class ShaderModule: public DeviceResource<VkShaderModule> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    ShaderModule(ShaderModule&& move) noexcept = default;
    ShaderModule& operator=(ShaderModule&& move_assign) = default;

    ~ShaderModule() override { destroy(); }

    static ShaderModule from_file(VK::Device* device, const std::string& filename, VkShaderModuleCreateFlags flags = 0) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if(!file.is_open()) {
            throw std::runtime_error("failed to open file");
        }
        std::string str;

        file.seekg(0, std::ios::end);
        str.reserve(file.tellg());
        file.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        return from_source_code(device, str, flags);
    }

    static ShaderModule from_source_code(VK::Device* device, const std::string& code, VkShaderModuleCreateFlags flags = 0) {
        VkShaderModuleCreateInfo create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.flags = flags;
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule handle = nullptr;

        if (vkCreateShaderModule(device->get_handle(), &create_info, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module");
        }

        return { device, handle };
    }

    void destroy() {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyShaderModule(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }
};

}