#pragma once

namespace VK {
class DescriptorSetLayoutBinding;
}

#include <vulkan/vulkan_core.h>
#include <vector>
#include "vk-descriptor-set-layout.hpp"

namespace VK {

class DescriptorSetLayoutFactory {
    VkDescriptorSetLayoutCreateInfo m_description {};
    std::vector<VkDescriptorSetLayoutBinding> m_bindings {};

public:
    DescriptorSetLayoutFactory();

    void set_flags(VkDescriptorSetLayoutCreateFlags flags) {
        m_description.flags = flags;
    }

    void bind_descriptor(uint32_t binding, DescriptorSetLayoutBinding& descriptor);

    DescriptorSetLayout create(VK::Device* device);
};

}