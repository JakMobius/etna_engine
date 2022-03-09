
#include <etna/vk-wrappers/device/vk-device-factory.hpp>

VK::DeviceFactory::DeviceFactory() {
    m_description.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    m_description.pEnabledFeatures = &m_features;
}

VK::Device VK::DeviceFactory::create(VK::PhysicalDevice* physical_device) {
    m_description.pQueueCreateInfos = m_queues_to_create.data();
    m_description.queueCreateInfoCount = m_queues_to_create.size();

    m_description.enabledExtensionCount = m_enabled_extensions.size();
    m_description.ppEnabledExtensionNames = m_enabled_extensions.data();

    m_description.enabledLayerCount = m_validation_layers.size();
    m_description.ppEnabledLayerNames = m_validation_layers.data();

    VkDevice device = nullptr;

    if (vkCreateDevice(physical_device->get_handle(), &m_description, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

    return { device, physical_device };
}

VK::DeviceFactory& VK::DeviceFactory::add_queue(VkDeviceQueueCreateFlags flags, uint32_t family, std::span<float> priorities) {
    m_queues_to_create.emplace_back();
    VkDeviceQueueCreateInfo& queue_create_info = m_queues_to_create.back();
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = family;
    queue_create_info.flags = flags;
    queue_create_info.queueCount = priorities.size();
    queue_create_info.pQueuePriorities = priorities.data();
    return *this;
}
