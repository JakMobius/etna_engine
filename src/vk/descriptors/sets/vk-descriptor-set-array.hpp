#pragma once

#include <vulkan/vulkan_core.h>
#include "../pool/vk-descriptor-pool.hpp"
#include "../vk-descriptor.hpp"

namespace VK {

class DescriptorSetArray {

    UnownedDescriptorPool m_descriptor_pool;
    std::vector<VkDescriptorSetLayout> m_layouts {};
    std::vector<VkDescriptorSet> m_descriptor_sets {};

public:
    explicit DescriptorSetArray(const UnownedDescriptorPool& descriptor_pool): m_descriptor_pool(descriptor_pool) {}

    std::vector<VkDescriptorSetLayout>& get_layouts() { return m_layouts; }
    const std::vector<VkDescriptorSet>& get_descriptor_sets() { return m_descriptor_sets; }

    void bind_descriptor(int descriptor_set_index, uint32_t binding, Descriptor& descriptor);

    void create();

    void free_sets() {
        if(m_descriptor_sets.empty() || m_descriptor_pool.is_null()) return;
        vkFreeDescriptorSets(m_descriptor_pool.get_device()->get_handle(), m_descriptor_pool.get_handle(), m_descriptor_sets.size(), m_descriptor_sets.data());
        m_descriptor_sets.clear();
    }
};

}