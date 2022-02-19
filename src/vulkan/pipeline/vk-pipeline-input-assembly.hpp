#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class PipelineInputAssembly {
    VkPipelineInputAssemblyStateCreateInfo m_description {};
public:
    PipelineInputAssembly(VkPrimitiveTopology topology, bool primitive_restart_enable) {
        m_description.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        m_description.topology = topology;
        m_description.primitiveRestartEnable = primitive_restart_enable ? VK_TRUE : VK_FALSE;
    }

    VkPipelineInputAssemblyStateCreateInfo& get_description() { return m_description; }
};

}