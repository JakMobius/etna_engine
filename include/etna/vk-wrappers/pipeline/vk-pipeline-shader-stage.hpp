#pragma once

#include <vector>
#include <etna/volk.hpp>
#include <etna/vk-wrappers/vk-shader.hpp>

namespace VK {

class PipelineShaderStage {
    VkPipelineShaderStageCreateInfo m_shader_stage {};
public:
    PipelineShaderStage() = default;
    void set_shader(const UnownedShaderModule& shader, VkShaderStageFlagBits stage, const char* entry_point = "main");

    const VkPipelineShaderStageCreateInfo& get_stage() { return m_shader_stage; }
};

}