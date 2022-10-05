#pragma once

#include <etna/volk.hpp>
#include <etna/vk-wrappers/device/vk-device.hpp>
#include <etna/vk-wrappers/resources/vk-device-resource.hpp>

namespace VK {

using UnownedShaderModule = UnownedDeviceResource<VkShaderModule>;

class ShaderModule: public DeviceResource<VkShaderModule> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    ShaderModule(ShaderModule&& move) noexcept = default;
    ShaderModule& operator=(ShaderModule&& move_assign) = default;

    ~ShaderModule() override { destroy(); }

    static ShaderModule from_file(VK::Device* device, const std::string& filename, VkShaderModuleCreateFlags flags = 0);

    static ShaderModule from_source_code(VK::Device* device, const std::string& code, VkShaderModuleCreateFlags flags = 0);

    void destroy() final;
};

}