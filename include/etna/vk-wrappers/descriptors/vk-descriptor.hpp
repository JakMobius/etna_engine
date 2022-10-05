#pragma once

#include <etna/volk.hpp>

namespace VK {

class Descriptor {
protected:
    VkWriteDescriptorSet m_description {};
public:
    Descriptor();

    VkWriteDescriptorSet& get_description() { return m_description; };
};

}