#pragma once

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include "codes/vk-result-code.hpp"
#include "codes/vk-object-type-code.hpp"
#include "codes/vk-severity-code.hpp"
#include "codes/vk-message-type-code.hpp"

struct vk_callback_parameters {
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity;
    VkDebugUtilsMessageTypeFlagsEXT message_type;
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data;
};

static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* p_user_data);

constexpr const char* VK_CREATE_DEBUG_UTILS_MESSENGER_EXT_NAME = "vkCreateDebugUtilsMessengerEXT";
constexpr const char* VK_DESTROY_DEBUG_UTILS_MESSENGER_EXT_NAME = "vkDestroyDebugUtilsMessengerEXT";

class VkDebugCallbackHandler {
    VkDebugUtilsMessageSeverityFlagBitsEXT m_min_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

    VkInstance m_instance = nullptr;
    VkDebugUtilsMessengerEXT m_handle = nullptr;

    VkResult create_messenger(const VkDebugUtilsMessengerCreateInfoEXT* create_info);
    VkResult destroy_messenger();

public:
    VkDebugCallbackHandler() {}
    ~VkDebugCallbackHandler() {
        if(m_handle) destroy_messenger();
    }

    VkDebugUtilsMessengerEXT get_handle() { return m_handle; }

    bool listen(VkInstance instance);
    void stop_listening();

    void dump_object(const VkDebugUtilsObjectNameInfoEXT* object) {

        std::cout << VK::ObjectTypeCode(object->objectType);

        if(object->pObjectName == nullptr) {
            std::cout << " ( unnamed )\n";
        } else {
            std::cout << " (" << object->pObjectName << ")\n";
        }
    }

    bool on_callback(vk_callback_parameters* callback_parameters) {
        if(callback_parameters->message_severity < m_min_severity) return false;

        std::cout << "Vulkan ["
                  << VK::SeverityCode(callback_parameters->message_severity) << "] ["
                  << VK::MessageTypeCode(callback_parameters->message_type) << "]: "
                  << callback_parameters->callback_data->pMessage << "\n";

        if(callback_parameters->message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            asm("nop");
        }

        if(callback_parameters->message_type == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
            asm("nop");
        }

        for(int i = 0; i < callback_parameters->callback_data->objectCount; i++) {
            std::cout << "\t- objects[" << i << "]: ";
            dump_object(&callback_parameters->callback_data->pObjects[i]);
            std::cout << "\n";
        }

        return false;
    }
};