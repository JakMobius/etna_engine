#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class Descriptor {
protected:
    VkWriteDescriptorSet m_description {};
public:
    Descriptor() {
        m_description.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    }

    VkWriteDescriptorSet& get_description() { return m_description; };
};

}