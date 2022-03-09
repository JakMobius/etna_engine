
#include <etna/vk-wrappers/pipeline/vk-pipeline-input-assembly-state.hpp>

VK::PipelineInputAssemblyStates::PipelineInputAssemblyStates() {
    m_description.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    m_description.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    m_description.primitiveRestartEnable = VK_FALSE;
}
