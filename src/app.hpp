#pragma once

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include "codes/vk-version-code.hpp"
#include "vk-debug-callback-handler.hpp"
#include "vk-queue-family-indices.hpp"

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

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
    VkBuffer m_vertex_buffer {};
    VkDeviceMemory m_vertex_buffer_memory {};
    VkBuffer m_index_buffer {};
    VkDeviceMemory m_index_buffer_memory {};
    VkDescriptorSetLayout m_descriptor_set_layout = {};
    VkDescriptorPool m_descriptor_pool {};
    std::vector<VkDescriptorSet> m_descriptor_sets {};

    VkImage m_texture_image {};
    VkDeviceMemory m_texture_image_memory {};
    VkImageView m_texture_image_view {};
    VkSampler m_texture_sampler {};

    VkImage m_depth_image {};
    VkDeviceMemory m_depth_image_memory {};
    VkImageView m_depth_image_view {};

    std::vector<float> m_vertex_buffer_storage {};
    std::vector<uint32_t> m_index_buffer_storage {};

    std::vector<VkBuffer> m_uniform_buffers {};
    std::vector<VkDeviceMemory> m_uniform_buffers_memory {};

    size_t m_current_frame = 0;

    const std::vector<const char*> required_validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> m_device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const bool m_enable_validation_layers = true;
    const bool m_enable_debug_messenger = true;

    float m_time = 0;;

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

    void setup_validation_layers();
    void create_instance();
    void setup_debug_messenger();
    void create_surface();
    void pick_gpu();
    void get_families();
    void create_logical_device();
    void create_swap_chain();
    void create_image_views();
    void create_render_pass();
    void create_descriptor_set_layout();
    void create_graphics_pipeline();
    void create_framebuffers();
    void create_command_pool();
    void create_mesh();
    void create_index_buffer();
    void create_vertex_buffer();
    void create_depth_resources();
    void create_texture_image();
    void create_texture_image_view();
    void create_texture_sampler();
    void create_uniform_buffers();
    void create_descriptor_pool();
    void create_descriptor_sets();
    void create_command_buffers();
    void create_sync_objects();

    void cleanup_swap_chain();
    void recreate_swap_chain();

    void update_uniform_buffer(uint32_t image_index);

    void draw_frame();

    VkPhysicalDevice select_best_gpu(const std::vector<VkPhysicalDevice>& devices);

    std::vector<const char*> get_required_extensions();

    bool check_validation_layer_support();

    void main_loop();

    void cleanup();

    bool is_device_suitable(VkPhysicalDevice device, VkPhysicalDeviceProperties* properties);

    bool check_device_extension_support(VkPhysicalDevice physical_device);

    VkShaderModule create_shader_module(const std::vector<char>& code);

    uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties);

    void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,
                       VkDeviceMemory &bufferMemory);

    void copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);

    void create_image(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                      VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &image_memory);

    VkCommandBuffer begin_single_time_commands();

    void end_single_time_commands(VkCommandBuffer command_buffer);

    void transition_image_layout(VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout);

    void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    VkImageView create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspect_flags);

    VkFormat
    find_supported_format(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkFormat find_depth_format();

    bool has_stencil_component(VkFormat format);
};