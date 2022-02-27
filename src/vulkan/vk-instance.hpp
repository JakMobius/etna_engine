
#include <vector>
#include <string>
#include <vulkan/vulkan_core.h>
#include "vk-physical-device.hpp"

namespace VK {

class Instance {

    VkInstance m_handle;

public:
    Instance(): m_handle(nullptr) {}
    explicit Instance(VkInstance handle): m_handle(handle) {}
    Instance(Instance&& move) noexcept : Instance(move.m_handle) { move.m_handle = nullptr; }
    Instance(const Instance& copy) = delete;
    Instance &operator=(Instance &&move_assign)  noexcept {
        if(this == &move_assign) return *this;
        destroy();
        m_handle = move_assign.m_handle;
        move_assign.m_handle = nullptr;
        return *this;
    }
    Instance& operator=(const Instance& copy_assign) = delete;

    ~Instance() { destroy(); }

    std::vector<VkLayerProperties> get_validation_layers() {
        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        std::vector<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
        return available_layers;
    }

    std::vector<PhysicalDevice> get_physical_devices() {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(m_handle, &device_count, nullptr);
        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(m_handle, &device_count, devices.data());

        std::vector<PhysicalDevice> physical_devices {devices.begin(), devices.end()};

        return physical_devices;
    }

    void destroy() {
        if(m_handle) {
            vkDestroyInstance(m_handle, nullptr);
            m_handle = nullptr;
        }
    }

    bool is_null() { return !m_handle; }
    explicit operator bool() { return m_handle != nullptr; }

    VkInstance get_handle() { return m_handle; }

};

}