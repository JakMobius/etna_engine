//
// Created by Артем on 09.02.2022.
//

#include <iostream>
#include "vk-debug-callback-handler.hpp"

VkBool32
vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                  VkDebugUtilsMessageTypeFlagsEXT message_type,
                  const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                  void* p_user_data) {

    vk_callback_parameters data {message_severity, message_type, callback_data };
    return ((VkDebugCallbackHandler*) p_user_data)->on_callback(&data) ? VK_TRUE : VK_FALSE;

}

VkResult VkDebugCallbackHandler::destroy_messenger() {
    if(!m_handle) return VK_INCOMPLETE;

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, VK_DESTROY_DEBUG_UTILS_MESSENGER_EXT_NAME);

    if(func != nullptr) {
        func(m_instance, m_handle, nullptr);
        m_handle = nullptr;
        return VK_SUCCESS;
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VkResult VkDebugCallbackHandler::create_messenger(const VkDebugUtilsMessengerCreateInfoEXT* create_info) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, VK_CREATE_DEBUG_UTILS_MESSENGER_EXT_NAME);

    if (func != nullptr) {
        return func(m_instance, create_info, nullptr, &m_handle);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

bool VkDebugCallbackHandler::listen(VkInstance instance) {
    if(!instance) return false;
    if(m_handle) return false;

    m_instance = instance;

    VkDebugUtilsMessageSeverityFlagsEXT severities[] = {
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
    };

    VkDebugUtilsMessageTypeFlagsEXT message_types =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.pfnUserCallback = vk_debug_callback;
    create_info.pUserData = this;

    for(auto& severity : severities) {
        if(severity >= m_min_severity) create_info.messageSeverity |= severity;
    }

    create_info.messageType = message_types;

    auto result = create_messenger(&create_info);

    if(result != VK_SUCCESS) {
        std::cout << "Error initializing debug messenger: " << VkResultCode(result) << "\n";
        return false;
    }
    return true;
}

void VkDebugCallbackHandler::stop_listening() {
    auto result = destroy_messenger();

    if(result != VK_SUCCESS) {
        std::cout << "Error deinitializing debug messenger: " << VkResultCode(result) << "\n";
    }
}
