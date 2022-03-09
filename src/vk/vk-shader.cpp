
#include <etna/vk-wrappers/vk-shader.hpp>
#include <fstream>
#include <sstream>

VK::ShaderModule
VK::ShaderModule::from_file(VK::Device* device, const std::string &filename, VkShaderModuleCreateFlags flags) {
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

VK::ShaderModule
VK::ShaderModule::from_source_code(VK::Device* device, const std::string &code, VkShaderModuleCreateFlags flags) {
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

void VK::ShaderModule::destroy() {
    if(!this->m_handle || !this->m_device) return;
    vkDestroyShaderModule(this->m_device->get_handle(), this->m_handle, nullptr);
    this->m_handle = nullptr;
}
