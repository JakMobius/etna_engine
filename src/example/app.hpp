#pragma once

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include "../vulkan/vulkan.hpp"
#include "../etna/basic-attachment-manager.hpp"
#include "../etna/swapchain-manager.hpp"
#include "../etna/image.hpp"
#include "../etna/command-queue/command-queue-pool.hpp"

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class Application {

    const int MAX_FRAMES_IN_FLIGHT = 2;

    VK::DebugCallbackHandler m_debug_callback_handler {};
    GLFWwindow* m_window = nullptr;
    const uint32_t m_window_width = 800;
    const uint32_t m_window_height = 600;

    VK::Instance m_instance;

    std::unique_ptr<VK::PhysicalDevice> m_physical_device {};
    VK::Device m_device;
    VK::CommandPool m_command_pool;
    std::unique_ptr<Etna::CommandQueuePool> m_graphics_command_queue_pool;

    int m_graphics_queue_family = -1;
    int m_present_queue_family = -1;

    VK::Queue m_device_graphics_queue {};
    VK::Queue m_device_present_queue {};

    VK::Surface m_surface {};
    std::unique_ptr<Etna::SwapchainManager> m_swapchain_manager {};
    std::unique_ptr<Etna::BasicAttachmentManager> m_attachment_manager = nullptr;

    std::vector<VK::UnownedFence> m_in_flight_images {};

    std::vector<VK::Semaphore> m_image_available_semaphores {};
    std::vector<VK::Semaphore> m_render_finished_semaphores {};
    std::vector<VK::Fence> m_in_flight_fences {};
    std::vector<VK::CommandBuffer> m_command_buffers {};

    std::vector<float> m_vertex_buffer_storage {};
    std::vector<uint32_t> m_index_buffer_storage {};

    std::vector<VK::MemoryBuffer> m_uniform_buffers {};

    VK::RenderPass m_render_pass {};
    VK::PipelineLayout m_pipeline_layout {};
    VK::Pipeline m_graphics_pipeline {};

    std::unique_ptr<VK::MemoryBuffer> m_vertex_buffer {};
    std::unique_ptr<VK::MemoryBuffer> m_index_buffer {};

    VK::DescriptorSetLayout m_descriptor_set_layout = {};
    VK::DescriptorPool m_descriptor_pool {};
    std::unique_ptr<VK::DescriptorSetArray> m_descriptor_set_array {};

    std::unique_ptr<Etna::Image> m_texture_image {};
    VK::Sampler m_texture_sampler {};
    int m_mip_levels = 0;

    VkSampleCountFlagBits m_msaa_samples = VK_SAMPLE_COUNT_1_BIT;

    std::unique_ptr<Etna::Image> m_color_image {};
    std::unique_ptr<Etna::Image> m_depth_image {};

    uint32_t m_current_frame = 0;

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

    ~Application() { cleanup(); }

private:

    void init_window();
    void init_vulkan();

    void create_instance();
    void create_logical_device();
    void create_swapchain();
    void create_render_pass();
    void create_descriptor_set_layout();
    void create_graphics_pipeline();
    void create_mesh();
    void create_index_buffer();
    void create_vertex_buffer();
    void create_depth_resources();
    void create_texture_image();
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

    void record_command_buffer(uint32_t frame_index, uint32_t swapchain_frame_index);

    std::vector<const char*> get_required_instance_extensions() const;

    void main_loop();

    void cleanup();

    bool is_device_suitable(const VK::PhysicalDevice* physical_device);

    VkFormat find_depth_format();

    void create_color_resources();

    void create_surface();
};