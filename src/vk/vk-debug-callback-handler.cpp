
#include <iostream>
#include <etna/vk-wrappers/vk-debug-callback-handler.hpp>

VkBool32 VK::vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                  VkDebugUtilsMessageTypeFlagsEXT message_type,
                  const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                  void* p_user_data) {

    CallbackParameters data {message_severity, message_type, callback_data };
    return ((DebugCallbackHandler*) p_user_data)->on_callback(&data) ? VK_TRUE : VK_FALSE;

}

void VK::DebugCallbackHandler::destroy() {
    if(!m_handle) return;

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, VK_DESTROY_DEBUG_UTILS_MESSENGER_EXT_NAME);

    if(func != nullptr) {
        func(m_instance, m_handle, nullptr);
        m_handle = nullptr;
    } else {
        throw std::runtime_error("could not destroy debug callback handler");
    }
}

VkResult VK::DebugCallbackHandler::create_messenger(const VkDebugUtilsMessengerCreateInfoEXT* create_info) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, VK_CREATE_DEBUG_UTILS_MESSENGER_EXT_NAME);

    if (func != nullptr) {
        return func(m_instance, create_info, nullptr, &m_handle);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

bool VK::DebugCallbackHandler::listen(VkInstance instance) {
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
        std::cout << "Error initializing debug messenger: " << VK::ResultCode(result) << "\n";
        return false;
    }
    return true;
}

void VK::DebugCallbackHandler::dump_object(const VkDebugUtilsObjectNameInfoEXT* object) {

    std::cout << VK::ObjectTypeCode(object->objectType);

    if(object->pObjectName == nullptr) {
        std::cout << " ( unnamed )\n";
    } else {
        std::cout << " (" << object->pObjectName << ")\n";
    }
}

bool VK::DebugCallbackHandler::on_callback(CallbackParameters* callback_parameters) {
    if(callback_parameters->message_severity < m_min_severity) return false;

    std::cout << "Vulkan ["
              << VK::SeverityCode(callback_parameters->message_severity) << "] ["
              << VK::MessageTypeCode(callback_parameters->message_type) << "]: "
              << callback_parameters->callback_data->pMessage << "\n";

//    if(callback_parameters->message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
//        asm("nop");
//    }
//
//    if(callback_parameters->message_type == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
//        asm("nop");
//    }

    for(int i = 0; i < callback_parameters->callback_data->objectCount; i++) {
        std::cout << "\t- objects[" << i << "]: ";
        dump_object(&callback_parameters->callback_data->pObjects[i]);
        std::cout << "\n";
    }

    return false;
}
