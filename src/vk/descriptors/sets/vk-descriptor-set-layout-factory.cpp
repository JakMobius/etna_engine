
#include <etna/vk-wrappers/descriptors/sets/vk-descriptor-set-layout-factory.hpp>
#include <etna/vk-wrappers/descriptors/vk-descriptor-set-layout-binding.hpp>

VK::DescriptorSetLayoutFactory::DescriptorSetLayoutFactory() {
    m_description.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
}

VK::DescriptorSetLayoutFactory& VK::DescriptorSetLayoutFactory::bind_descriptor(uint32_t binding, VK::DescriptorSetLayoutBinding &descriptor) {
    auto& description = descriptor.get_description();
    description.binding = binding;
    m_bindings.push_back(description);
    return *this;
}

VK::DescriptorSetLayout VK::DescriptorSetLayoutFactory::create(VK::Device* device) {

    m_description.bindingCount = m_bindings.size();
    m_description.pBindings = m_bindings.data();

    VkDescriptorSetLayout layout = nullptr;

    if (vkCreateDescriptorSetLayout(device->get_handle(), &m_description, nullptr, &layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout");
    }

    return { device, layout };
}
