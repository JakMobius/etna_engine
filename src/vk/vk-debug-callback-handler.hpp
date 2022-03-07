#pragma once

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include "codes/vk-result-code.hpp"
#include "codes/vk-object-type-code.hpp"
#include "codes/vk-severity-code.hpp"
#include "codes/vk-message-type-code.hpp"

namespace VK {
struct CallbackParameters {
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

class DebugCallbackHandler {
    VkDebugUtilsMessageSeverityFlagBitsEXT m_min_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

    VkInstance m_instance = nullptr;
    VkDebugUtilsMessengerEXT m_handle = nullptr;

    VkResult create_messenger(const VkDebugUtilsMessengerCreateInfoEXT* create_info);

    VkResult destroy_messenger();

public:
    DebugCallbackHandler() {}

    ~DebugCallbackHandler() {
        if(m_handle) destroy_messenger();
    }

    VkDebugUtilsMessengerEXT get_handle() { return m_handle; }

    bool listen(VkInstance instance);

    void stop_listening();

    void dump_object(const VkDebugUtilsObjectNameInfoEXT* object);

    bool on_callback(CallbackParameters* callback_parameters);
};
}