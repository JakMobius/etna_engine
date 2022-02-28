#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class PipelineInputAssemblyStates {
    VkPipelineInputAssemblyStateCreateInfo m_description {};
public:
    PipelineInputAssemblyStates();

    void set_topology(VkPrimitiveTopology topology) {
        m_description.topology = topology;
    }

    void set_primitive_restart_enable(bool primitive_restart_enable) {
        m_description.primitiveRestartEnable = primitive_restart_enable;
    }

    VkPipelineInputAssemblyStateCreateInfo& get_description() { return m_description; }
};

}