#pragma once

#include <etna/volk.hpp>

namespace VK {

class PipelineMultisamplingState {
    VkPipelineMultisampleStateCreateInfo m_description {};

public:
    PipelineMultisamplingState();

    PipelineMultisamplingState& set_sample_shading_enable(VkBool32 sample_shading_enable) {
        m_description.sampleShadingEnable = sample_shading_enable;
        return *this;
    }

    PipelineMultisamplingState& set_min_sample_shading(float min_sample_shading) {
        m_description.minSampleShading = min_sample_shading;
        return *this;
    }

    PipelineMultisamplingState& set_rasterization_samples(VkSampleCountFlagBits rasterization_samples) {
        m_description.rasterizationSamples = rasterization_samples;
        return *this;
    }

    PipelineMultisamplingState& set_sample_mask(const VkSampleMask* sample_mask) {
        m_description.pSampleMask = sample_mask;
        return *this;
    }

    PipelineMultisamplingState& set_alpha_to_coverage_enable(VkBool32 alpha_to_coverage_enable) {
        m_description.alphaToCoverageEnable = alpha_to_coverage_enable;
        return *this;
    }

    PipelineMultisamplingState& set_alpha_to_one_enable(VkBool32 alpha_to_one_enable) {
        m_description.alphaToOneEnable = alpha_to_one_enable;
        return *this;
    }

    VkPipelineMultisampleStateCreateInfo& get_description() {
        return m_description;
    }
};

}
