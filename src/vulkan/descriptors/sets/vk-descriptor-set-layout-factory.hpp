#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>
#include "vk-descriptor-set-layout.hpp"
#include "../vk-descriptor-set-layout-binding.hpp"

namespace VK {

class DescriptorSetLayoutFactory {
    VkDescriptorSetLayoutCreateInfo m_description {};
    std::vector<VkDescriptorSetLayoutBinding> m_bindings {};

public:
    DescriptorSetLayoutFactory() {
        m_description.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        m_description.bindingCount = 0;
        m_description.pBindings = nullptr;
    }

    void set_flags(VkDescriptorSetLayoutCreateFlags flags) {
        m_description.flags = flags;
    }

    void bind_descriptor(uint32_t binding, DescriptorSetLayoutBinding& descriptor) {
        auto& description = descriptor.get_description();
        description.binding = binding;
        m_bindings.push_back(description);
    }

    DescriptorSetLayout create(VK::Device* device) {

        m_description.bindingCount = m_bindings.size();
        m_description.pBindings = m_bindings.data();

        VkDescriptorSetLayout layout = nullptr;

        if (vkCreateDescriptorSetLayout(device->get_handle(), &m_description, nullptr, &layout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout");
        }

        return { device, layout };
    }
};

}