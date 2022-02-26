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

    DescriptorPool create(Device* device) {
        m_pool_info.poolSizeCount = m_pool_sizes.size();
        m_pool_info.pPoolSizes = m_pool_sizes.data();

        VkDescriptorPool descriptor_pool = nullptr;

        if (vkCreateDescriptorPool(device->get_handle(), &m_pool_info, nullptr, &descriptor_pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool");
        }

        return { device, descriptor_pool };
    }
};

}