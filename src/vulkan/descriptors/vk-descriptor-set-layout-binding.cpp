//
// Created by Артем on 28.02.2022.
//

#include "vk-descriptor-set-layout-binding.hpp"

VK::DescriptorSetLayoutBinding::DescriptorSetLayoutBinding(VkDescriptorType type) {
    set_descriptor_type(type);
    set_descriptor_count(1);
}
