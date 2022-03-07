#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "../vk-shader.hpp"

namespace VK {

class PipelineShaderStage {
    std::vector <VkPipelineShaderStageCreateInfo> m_shader_stages {};
public:

    PipelineShaderStage() {}

    void add_shader(ShaderModule &shader, VkShaderStageFlagBits stage);

    const std::vector <VkPipelineShaderStageCreateInfo> &get_shader_stages() {
        return m_shader_stages;
    }
};

}