
#include <etna/vk-wrappers/descriptors/vk-descriptor.hpp>

VK::Descriptor::Descriptor() {
    m_description.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
}
