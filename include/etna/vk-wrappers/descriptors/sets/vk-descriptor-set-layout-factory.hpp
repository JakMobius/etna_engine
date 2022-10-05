#pragma once

namespace VK {
class DescriptorSetLayoutBinding;
}

#include <etna/volk.hpp>
#include <vector>
#include "vk-descriptor-set-layout.hpp"

namespace VK {

class DescriptorSetLayoutFactory {
    VkDescriptorSetLayoutCreateInfo m_description {};
    std::vector<VkDescriptorSetLayoutBinding> m_bindings {};

public:
    DescriptorSetLayoutFactory();

    DescriptorSetLayoutFactory& set_flags(VkDescriptorSetLayoutCreateFlags flags) {
        m_description.flags = flags;
        return *this;
    }

    DescriptorSetLayoutFactory& bind_descriptor(uint32_t binding, DescriptorSetLayoutBinding& descriptor);

    DescriptorSetLayout create(VK::Device* device);
};

}