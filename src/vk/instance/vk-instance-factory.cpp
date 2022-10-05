
#include <etna/vk-wrappers/instance/vk-instance-factory.hpp>

VK::InstanceFactory::InstanceFactory() {
    m_description.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    m_application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    m_description.pApplicationInfo = &m_application_info;

    set_app_engine_name("ETNA");
    set_app_engine_version(VersionCode {1, 0, 0});
    set_app_name("Unnamed App");
    set_app_version(VersionCode {1, 0, 0});
    set_app_api_version(VersionCode {1, 0, 0});
}

VK::Instance VK::InstanceFactory::create() {
    m_description.ppEnabledLayerNames = enabled_layer_names.data();
    m_description.enabledLayerCount = enabled_layer_names.size();
    m_description.ppEnabledExtensionNames = enabled_extension_names.data();
    m_description.enabledExtensionCount = enabled_extension_names.size();

    VkInstance instance = nullptr;

    if (vkCreateInstance(&m_description, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    volkLoadInstance(instance);

    return Instance { instance };
}
