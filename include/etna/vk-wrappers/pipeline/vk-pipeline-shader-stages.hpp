#pragma once

#include <vector>
#include <etna/volk.hpp>
#include <etna/vk-wrappers/vk-shader.hpp>

namespace VK {

class PipelineShaderStages {
    std::vector <VkPipelineShaderStageCreateInfo> m_shader_stages {};
public:

    PipelineShaderStages() {}

    void add_shader(ShaderModule &shader, VkShaderStageFlagBits stage, const char* entry_point = "main");

    const std::vector <VkPipelineShaderStageCreateInfo> &get_shader_stages() {
        return m_shader_stages;
    }
};

}