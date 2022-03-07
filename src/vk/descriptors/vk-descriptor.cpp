//
// Created by Артем on 26.02.2022.
//

#include "vk-descriptor.hpp"

VK::Descriptor::Descriptor() {
    m_description.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
}
