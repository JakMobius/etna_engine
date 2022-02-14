//
// Created by Артем on 14.02.2022.
//

#include "vk-surface-context.hpp"

VK::SurfaceContext::SurfaceContext(VK::PhysicalDevice* device, VkSurfaceKHR surface) :
        m_physical_device(device),
        m_surface(surface) {

    auto queue_family_indices = m_physical_device->get_queue_family_indices();

    m_graphics_queue_index = queue_family_indices->find_family(VK_QUEUE_GRAPHICS_BIT);
    m_present_queue_index = queue_family_indices->find_surface_present_family(surface);
}


void VK::SurfaceContext::create_command_pool() {
    VkCommandPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = get_graphics_queue_index();
    pool_info.flags = 0; // Optional

    VkCommandPool command_pool;

    if (vkCreateCommandPool(get_device()->get_handle(), &pool_info, nullptr, &command_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool");
    }

    m_command_pool = std::make_unique<VK::CommandPool>(command_pool, this);
}

void VK::SurfaceContext::create_logical_device(const std::vector<const char*>& extensions,
                                               const std::vector<const char*>& validation_layers) {
    std::vector<VkDeviceQueueCreateInfo> queues_to_create {};

    float queue_priority = 1.0f;

    for (int queue_family : get_queue_families()) {
        queues_to_create.emplace_back();
        VkDeviceQueueCreateInfo& queue_create_info = queues_to_create.back();

        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
    }

    VkPhysicalDeviceFeatures device_features {};
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    create_info.pQueueCreateInfos = queues_to_create.data();
    create_info.queueCreateInfoCount = queues_to_create.size();
    create_info.pEnabledFeatures = &device_features;

    create_info.enabledExtensionCount = extensions.size();
    create_info.ppEnabledExtensionNames = extensions.data();

    create_info.enabledLayerCount = validation_layers.size();
    create_info.ppEnabledLayerNames = validation_layers.data();

    VkDevice device = nullptr;

    if (vkCreateDevice(m_physical_device->get_handle(), &create_info, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

    m_logical_device = std::make_unique<VK::Device>(device, m_physical_device);

    vkGetDeviceQueue(m_logical_device->get_handle(), get_graphics_queue_index(), 0, &m_graphics_queue);
    vkGetDeviceQueue(m_logical_device->get_handle(), get_present_queue_index(), 0, &m_present_queue);
}