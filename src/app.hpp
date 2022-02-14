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
#include "vulkan/codes/vk-version-code.hpp"
#include "vulkan/vk-debug-callback-handler.hpp"
#include "vulkan/vk-queue-family-indices.hpp"
#include "vulkan/vk-memory.hpp"
#include "vulkan/vk-buffer.hpp"
#include "vulkan/vk-surface-context.hpp"
#include "vulkan/vk-image-2d.hpp"

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

    std::unique_ptr<VK::PhysicalDevice> m_physical_device {};
    std::unique_ptr<VK::SurfaceContext> m_surface_context {};

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
    std::vector<std::unique_ptr<VK::CommandBuffer>> m_command_buffers {};
    std::vector<VkSemaphore> m_image_available_semaphores {};
    std::vector<VkSemaphore> m_render_finished_semaphores {};

    std::vector<VkFence> m_in_flight_fences {};
    std::vector<VkFence> m_in_flight_images {};

    std::unique_ptr<VK::Memory> m_vertex_buffer_memory {};
    std::unique_ptr<VK::Buffer> m_vertex_buffer {};

    std::unique_ptr<VK::Memory> m_index_buffer_memory {};
    std::unique_ptr<VK::Buffer> m_index_buffer {};

    std::vector<std::unique_ptr<VK::Buffer>> m_uniform_buffers {};
    std::vector<std::unique_ptr<VK::Memory>> m_uniform_buffers_memory {};

    VkDescriptorSetLayout m_descriptor_set_layout = {};
    VkDescriptorPool m_descriptor_pool {};
    std::vector<VkDescriptorSet> m_descriptor_sets {};

    std::unique_ptr<VK::Image2D> m_texture_image {};
    VK::Memory m_texture_image_memory {};
    VkImageView m_texture_image_view {};
    VkSampler m_texture_sampler {};
    int m_mip_levels = 0;

    VkSampleCountFlagBits m_msaa_samples = VK_SAMPLE_COUNT_1_BIT;

    std::unique_ptr<VK::Image2D> m_color_image;
    VK::Memory m_color_image_memory;
    VkImageView m_color_image_view;

    VK::Memory m_depth_image_memory {};
    std::unique_ptr<VK::Image2D> m_depth_image {};
    VkImageView m_depth_image_view {};

    std::vector<float> m_vertex_buffer_storage {};
    std::vector<uint32_t> m_index_buffer_storage {};

    size_t m_current_frame = 0;

    const std::vector<const char*> m_required_validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> m_device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const bool m_enable_validation_layers = true;
    const bool m_enable_debug_messenger = true;

    glm::vec3 m_camera_pos {};
    glm::vec3 m_camera_direction {};
    float m_pitch = 0;
    float m_yaw = 0;

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

    VK::PhysicalDevice get_best_physical_device();

    void update_uniform_buffer(uint32_t image_index);

    void draw_frame();

    const VK::PhysicalDevice* select_best_physical_device(const std::vector<VK::PhysicalDevice>& devices);

    std::vector<const char*> get_required_extensions();

    bool check_validation_layer_support();

    void main_loop();

    void cleanup();

    bool is_device_suitable(const VK::PhysicalDevice* physical_device);

    VkShaderModule create_shader_module(const std::vector<char>& code);

    void copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);

    void transition_image_layout(VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout, int mip_levels);

    void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    VkImageView create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspect_flags, int mip_levels);

    VkFormat find_depth_format();

    bool has_stencil_component(VkFormat format);

    void generate_mipmaps(VkImage image, VkFormat image_format, int32_t tex_width, int32_t tex_height, uint32_t mip_levels);

    void create_color_resources();
};