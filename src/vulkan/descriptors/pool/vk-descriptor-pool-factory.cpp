//
// Created by Артем on 26.02.2022.
//

#include "vk-descriptor-pool-factory.hpp"


VK::DescriptorPool VK::DescriptorPoolFactory::create(VK::Device* device) {
    m_pool_info.poolSizeCount = m_pool_sizes.size();
    m_pool_info.pPoolSizes = m_pool_sizes.data();

    VkDescriptorPool descriptor_pool = nullptr;

    if (vkCreateDescriptorPool(device->get_handle(), &m_pool_info, nullptr, &descriptor_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool");
    }

    return { device, descriptor_pool };
}
