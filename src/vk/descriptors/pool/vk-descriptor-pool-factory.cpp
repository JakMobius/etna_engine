
#include <etna/vk-wrappers/descriptors/pool/vk-descriptor-pool-factory.hpp>


VK::DescriptorPool VK::DescriptorPoolFactory::create(VK::Device* device) {
    m_description.poolSizeCount = m_pool_sizes.size();
    m_description.pPoolSizes = m_pool_sizes.data();

    VkDescriptorPool descriptor_pool = nullptr;

    if (vkCreateDescriptorPool(device->get_handle(), &m_description, nullptr, &descriptor_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool");
    }

    return { device, descriptor_pool };
}
