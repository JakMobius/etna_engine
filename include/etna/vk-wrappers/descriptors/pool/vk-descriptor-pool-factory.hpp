#pragma once

#include <vector>
#include <etna/volk.hpp>
#include "vk-descriptor-pool.hpp"

namespace VK {

class DescriptorPoolFactory {
    VkDescriptorPoolCreateInfo m_description {};
    std::vector<VkDescriptorPoolSize> m_pool_sizes {};

public:
    DescriptorPoolFactory() {
        m_description.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    }

    DescriptorPoolFactory& set_flags(VkDescriptorPoolCreateFlags flags) {
        m_description.flags = flags;
        return *this;
    }

    DescriptorPoolFactory& add_pool_size(VkDescriptorType type, uint32_t count) {
        m_pool_sizes.push_back({type, count});
        return *this;
    }

    DescriptorPoolFactory& set_max_sets(uint32_t max_sets) {
        m_description.maxSets = max_sets;
        return *this;
    }

    DescriptorPool create(Device* device);
};

}