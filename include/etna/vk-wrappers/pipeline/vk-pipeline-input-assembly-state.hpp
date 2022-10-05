#pragma once

#include <etna/volk.hpp>

namespace VK {

class PipelineInputAssemblyStates {
    VkPipelineInputAssemblyStateCreateInfo m_description {};
public:
    PipelineInputAssemblyStates();

    PipelineInputAssemblyStates& set_topology(VkPrimitiveTopology topology) {
        m_description.topology = topology;
        return *this;
    }

    PipelineInputAssemblyStates& set_primitive_restart_enable(bool primitive_restart_enable) {
        m_description.primitiveRestartEnable = primitive_restart_enable;
        return *this;
    }

    VkPipelineInputAssemblyStateCreateInfo& get_description() { return m_description; }
};

}