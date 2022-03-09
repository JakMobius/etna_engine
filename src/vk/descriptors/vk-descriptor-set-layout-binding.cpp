
#include <etna/vk-wrappers/descriptors/vk-descriptor-set-layout-binding.hpp>

VK::DescriptorSetLayoutBinding::DescriptorSetLayoutBinding(VkDescriptorType type) {
    set_descriptor_type(type);
    set_descriptor_count(1);
}
