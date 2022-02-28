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
    InstanceFactory();

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

    Instance create();

    std::vector<const char*>& get_enabled_layer_names() { return enabled_layer_names; }
    std::vector<const char*>& get_enabled_extension_names() { return enabled_extension_names; }
};

}