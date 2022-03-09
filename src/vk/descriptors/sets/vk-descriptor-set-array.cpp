
#include <etna/vk-wrappers/descriptors/sets/vk-descriptor-set-array.hpp>
#include <etna/vk-wrappers/descriptors/vk-descriptor.hpp>

void VK::DescriptorSetArray::bind_descriptor(int descriptor_set_index, uint32_t binding, VK::Descriptor &descriptor) {
    auto& description = descriptor.get_description();
    description.dstSet = m_descriptor_sets[descriptor_set_index];
    description.dstBinding = binding;

    // TODO: batch update calls
    vkUpdateDescriptorSets(m_descriptor_pool.get_device()->get_handle(), 1, &description, 0, nullptr);
}

void VK::DescriptorSetArray::create() {
    VkDescriptorSetAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = m_descriptor_pool.get_handle();
    alloc_info.descriptorSetCount = m_layouts.size();
    alloc_info.pSetLayouts = m_layouts.data();

    m_descriptor_sets.resize(m_layouts.size());
    if (vkAllocateDescriptorSets(m_descriptor_pool.get_device()->get_handle(), &alloc_info, m_descriptor_sets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets");
    }
}
