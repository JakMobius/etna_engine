
#include <etna/volk.hpp>
#include <etna/vk-wrappers/codes/vk-physical-device-type-code.hpp>

const char* VK::PhysicalDeviceTypeCode::name(VkPhysicalDeviceType code) {
    switch(code) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER: return "OTHER";
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "INTEGRATED_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "DISCRETE_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "VIRTUAL_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_CPU: return "CPU";
        default: return "unknown device type";
    }
}

std::ostream &operator<<(std::ostream &stream, const VK::PhysicalDeviceTypeCode &result) {
    stream << VK::PhysicalDeviceTypeCode::name(result.get_code());
    return stream;
}
