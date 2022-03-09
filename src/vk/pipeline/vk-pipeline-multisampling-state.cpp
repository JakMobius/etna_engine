
#include <etna/vk-wrappers/pipeline/vk-pipeline-multisampling-state.hpp>

VK::PipelineMultisamplingState::PipelineMultisamplingState() {
    m_description.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    m_description.sampleShadingEnable = VK_FALSE;
    m_description.minSampleShading = 1.0f;
    m_description.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    m_description.pSampleMask = nullptr;
    m_description.alphaToCoverageEnable = VK_FALSE;
    m_description.alphaToOneEnable = VK_FALSE;
}
