#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class PipelineMultisamplingState {
    VkPipelineMultisampleStateCreateInfo m_description {};

public:
    PipelineMultisamplingState() {
        m_description.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        m_description.sampleShadingEnable = VK_FALSE;
        m_description.minSampleShading = 1.0f;
        m_description.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        m_description.pSampleMask = nullptr;
        m_description.alphaToCoverageEnable = VK_FALSE;
        m_description.alphaToOneEnable = VK_FALSE;
    }

    void set_sample_shading_enable(VkBool32 sample_shading_enable) {
        m_description.sampleShadingEnable = sample_shading_enable;
    }
    void set_min_sample_shading(float min_sample_shading) {
        m_description.minSampleShading = min_sample_shading;
    }
    void set_rasterization_samples(VkSampleCountFlagBits rasterization_samples) {
        m_description.rasterizationSamples = rasterization_samples;
    }
    void set_sample_mask(const VkSampleMask* sample_mask) {
        m_description.pSampleMask = sample_mask;
    }
    void set_alpha_to_coverage_enable(VkBool32 alpha_to_coverage_enable) {
        m_description.alphaToCoverageEnable = alpha_to_coverage_enable;
    }
    void set_alpha_to_one_enable(VkBool32 alpha_to_one_enable) {
        m_description.alphaToOneEnable = alpha_to_one_enable;
    }

    VkPipelineMultisampleStateCreateInfo& get_description() {
        return m_description;
    }
};

}
