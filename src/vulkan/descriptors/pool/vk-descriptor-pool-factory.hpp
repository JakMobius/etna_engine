#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "vk-descriptor-pool.hpp"

namespace VK {

class DescriptorPoolFactory {
    VkDescriptorPoolCreateInfo m_description {};
    std::vector<VkDescriptorPoolSize> m_pool_sizes {};

public:
    DescriptorPoolFactory() {
        m_description.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    }

    void set_flags(VkDescriptorPoolCreateFlags flags) {
        m_description.flags = flags;
    }

    void add_pool_size(VkDescriptorType type, uint32_t count) {
        m_pool_sizes.push_back({type, count});
    }

    void set_max_sets(uint32_t max_sets) {
        m_description.maxSets = max_sets;
    }

    DescriptorPool create(Device* device);
};

}