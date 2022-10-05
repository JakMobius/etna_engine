
#include <etna/vk-wrappers/instance/vk-instance.hpp>
#include <etna/vk-wrappers/device/vk-physical-device.hpp>

std::vector<VkLayerProperties> VK::InstanceBase::get_supported_validation_layers() {
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
    return available_layers;
}

std::vector<VK::PhysicalDevice> VK::InstanceBase::get_physical_devices() {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(m_handle, &device_count, nullptr);
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(m_handle, &device_count, devices.data());

    std::vector<PhysicalDevice> physical_devices {devices.begin(), devices.end()};

    return physical_devices;
}

std::vector<VkExtensionProperties> VK::InstanceBase::get_extensions() {
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
    return extensions;
}

void VK::Instance::destroy() {
    if(!m_handle) return;
    vkDestroyInstance(m_handle, nullptr);
    m_handle = nullptr;
}
