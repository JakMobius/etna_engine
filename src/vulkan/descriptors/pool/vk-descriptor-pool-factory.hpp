#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "vk-descriptor-pool.hpp"

namespace VK {

class DescriptorPoolFactory {
    VkDescriptorPoolCreateInfo m_pool_info {};
    std::vector<VkDescriptorPoolSize> m_pool_sizes {};

public:
    DescriptorPoolFactory() {
        m_pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    }

    void add_pool_size(VkDescriptorType type, uint32_t count) {
        m_pool_sizes.push_back({type, count});
    }

    void set_max_sets(uint32_t max_sets) {
        m_pool_info.maxSets = max_sets;
    }

    DescriptorPool create(Device* device);
};

}