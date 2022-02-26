#pragma once

#include "../vk-device.hpp"
#include "vk-descriptor.hpp"

namespace VK {

class DescriptorSetArray {

    VK::Device* m_device;
    VkDescriptorPool m_descriptor_pool;
    std::vector<VkDescriptorSetLayout> m_layouts {};
    std::vector<VkDescriptorSet> m_descriptor_sets {};

public:
    DescriptorSetArray(VK::Device* device, VkDescriptorPool descriptor_pool):
        m_device(device), m_descriptor_pool(descriptor_pool) {

    }

    std::vector<VkDescriptorSetLayout>& get_layouts() { return m_layouts; }
    const std::vector<VkDescriptorSet>& get_descriptor_sets() { return m_descriptor_sets; }

    void bind_descriptor(int descriptor_set_index, uint32_t binding, Descriptor& descriptor) {
        auto& description = descriptor.get_description();
        description.dstSet = m_descriptor_sets[descriptor_set_index];
        description.dstBinding = binding;

        // TODO: batch update calls
        vkUpdateDescriptorSets(m_device->get_handle(), 1, &description, 0, nullptr);
    }

    void create() {
        VkDescriptorSetAllocateInfo alloc_info {};
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.descriptorPool = m_descriptor_pool;
        alloc_info.descriptorSetCount = m_layouts.size();
        alloc_info.pSetLayouts = m_layouts.data();

        m_descriptor_sets.resize(m_layouts.size());
        if (vkAllocateDescriptorSets(m_device->get_handle(), &alloc_info, m_descriptor_sets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets");
        }
    }
};

}