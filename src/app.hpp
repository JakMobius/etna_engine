#pragma once

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "codes/vk-version-code.hpp"
#include "vk-debug-callback-handler.hpp"
#include "vk-queue-family-indices.hpp"

// TODO: Check if exceptions are used safely in this class

class HelloTriangleApplication {

    const int MAX_FRAMES_IN_FLIGHT = 2;

    VkDebugCallbackHandler m_debug_callback_handler {};
    GLFWwindow* m_window = nullptr;
    const uint32_t m_window_width = 800;
    const uint32_t m_window_height = 600;
    VkInstance m_instance {};
    VkPhysicalDevice m_gpu {};
    VkDevice m_device {};
    VkQueue m_device_graphics_queue {};
    VkQueue m_device_present_queue {};
    VkSurfaceKHR m_surface {};

    VkSwapchainKHR m_swap_chain {};
    std::vector<VkImage> m_swap_chain_images {};
    VkFormat m_swap_chain_image_format {};
    VkExtent2D m_swap_chain_extent {};
    std::vector<VkImageView> m_swap_chain_image_views {};
    VkRenderPass m_render_pass {};
    VkPipelineLayout m_pipeline_layout {};
    VkPipeline m_graphics_pipeline {};
    std::vector<VkFramebuffer> m_swap_chain_framebuffers {};
    VkCommandPool m_command_pool {};
    std::vector<VkCommandBuffer> m_command_buffers {};
    std::vector<VkSemaphore> m_image_available_semaphores {};
    std::vector<VkSemaphore> m_render_finished_semaphores {};
    std::unique_ptr<VkQueueFamilyIndices> m_family_indices {};
    std::vector<VkFence> m_in_flight_fences {};
    std::vector<VkFence> m_in_flight_images {};

    size_t m_current_frame = 0;

    const std::vector<const char*> required_validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> m_device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const bool enable_validation_layers = true;
    const bool enable_debug_messenger = true;


public:

    bool m_framebuffer_resized = false;

    void run() {
        init_window();
        init_vulkan();
        main_loop();
        cleanup();
    }

private:
    void init_window();
    void init_vulkan();
    void pick_gpu();
    void get_families();
    void create_logical_device();
    void create_surface();
    void create_swap_chain();
    void create_image_views();
    void create_graphics_pipeline();
    void create_render_pass();
    void create_framebuffers();
    void create_command_pool();
    void create_command_buffers();
    void create_semaphores();

    void cleanup_swap_chain();
    void recreate_swap_chain();

    void draw_frame();

    VkPhysicalDevice select_best_gpu(const std::vector<VkPhysicalDevice>& devices);

    std::vector<const char*> get_required_extensions();

    void create_instance();

    bool check_validation_layer_support();

    void main_loop();

    void cleanup();

    bool is_device_suitable(VkPhysicalDevice device, VkPhysicalDeviceProperties* properties);

    void setup_debug_messenger();

    void setup_validation_layers();

    bool check_device_extension_support(VkPhysicalDevice physical_device);

    VkShaderModule create_shader_module(const std::vector<char>& code);
};