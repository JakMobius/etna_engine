#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
#include "../codes/vk-version-code.hpp"
#include "vk-instance.hpp"

namespace VK {

class InstanceFactory {
    VkInstanceCreateInfo m_description {};
    VkApplicationInfo m_application_info {};
    std::vector<const char*> enabled_layer_names {};
    std::vector<const char*> enabled_extension_names {};

public:
    InstanceFactory() {
        m_description.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        m_application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

        m_description.pApplicationInfo = &m_application_info;

        set_app_engine_name("ETNA");
        set_app_engine_version(VersionCode {1, 0, 0});
        set_app_name("Unnamed App");
        set_app_version(VersionCode {1, 0, 0});
        set_app_api_version(VersionCode {1, 0, 0});
    }

    void set_app_api_version(VersionCode version) {
        m_application_info.applicationVersion = version.get_code();
    }

    void set_app_name(const char* app_name) {
        m_application_info.pApplicationName = app_name;
    }

    void set_app_version(VersionCode version) {
        m_application_info.applicationVersion = version.get_code();
    }

    void set_app_engine_name(const char* name) {
        m_application_info.pEngineName = name;
    }

    void set_app_engine_version(VersionCode version) {
        m_application_info.engineVersion = version.get_code();
    }

    void set_create_flags(VkInstanceCreateFlags flags) {
        m_description.flags = flags;
    }

    Instance create() {
        m_description.ppEnabledLayerNames = enabled_layer_names.data();
        m_description.enabledLayerCount = enabled_layer_names.size();
        m_description.ppEnabledExtensionNames = enabled_extension_names.data();
        m_description.enabledExtensionCount = enabled_extension_names.size();

        VkInstance instance = nullptr;

        if (vkCreateInstance(&m_description, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }

        return Instance { instance };
    }

    std::vector<const char*>& get_enabled_layer_names() { return enabled_layer_names; }
    std::vector<const char*>& get_enabled_extension_names() { return enabled_extension_names; }
};

}