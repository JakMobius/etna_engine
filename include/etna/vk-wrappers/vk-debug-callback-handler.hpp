#pragma once

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include <etna/vk-wrappers/codes/vk-result-code.hpp>
#include <etna/vk-wrappers/codes/vk-object-type-code.hpp>
#include <etna/vk-wrappers/codes/vk-severity-code.hpp>
#include <etna/vk-wrappers/codes/vk-message-type-code.hpp>

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

public:
    DebugCallbackHandler() {}

    ~DebugCallbackHandler() {
        destroy();
    }

    VkDebugUtilsMessengerEXT get_handle() { return m_handle; }

    bool listen(VkInstance instance);

    void dump_object(const VkDebugUtilsObjectNameInfoEXT* object);

    bool on_callback(CallbackParameters* callback_parameters);

    void destroy();
};
}