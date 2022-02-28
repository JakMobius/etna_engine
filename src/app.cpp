//
// Created by Артем on 09.02.2022.
//

#include <FreeImage.h>
#include <random>
#include <chrono>
#include <thread>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "app.hpp"

void Application::create_instance() {
    VK::InstanceFactory factory;

    factory.set_app_name("ETNA example");
    factory.set_app_version({1, 0, 0});
    factory.get_enabled_extension_names() = get_required_extensions();

    if (m_enable_validation_layers) {
        factory.get_enabled_layer_names() = m_required_validation_layers;
    }

    m_instance = factory.create();

    create_surface();

    if (m_enable_validation_layers && !check_validation_layer_support()) {
        throw std::runtime_error("some requested validation layers are not available");
    }

    if(m_enable_debug_messenger && !m_debug_callback_handler.listen(m_instance.get_handle())) {
        throw std::runtime_error("debug callback handler failed to initialize");
    }
}

void Application::create_surface() {
    VkSurfaceKHR surface = nullptr;
    if (glfwCreateWindowSurface(m_instance.get_handle(), m_window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }
    m_surface = VK::Surface {&m_instance, surface };
}

std::vector<const char*> Application::get_required_extensions() const {
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions = nullptr;

    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if(m_enable_validation_layers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void Application::init_vulkan() {
    create_instance();
    create_logical_device();
    create_swap_chain();
    create_mesh();
    create_index_buffer();
    create_vertex_buffer();
    create_color_resources();
    create_depth_resources();
    create_render_pass();
    create_swapchain_images();
    create_descriptor_set_layout();
    create_graphics_pipeline();
    create_texture_image();
    create_texture_sampler();
    create_uniform_buffers();
    create_descriptor_pool();
    create_descriptor_sets();
    create_sync_objects();
    create_command_buffers();
}

VK::PhysicalDevice Application::get_best_physical_device() {
    auto physical_devices = m_instance.get_physical_devices();

    const VK::PhysicalDevice* any_suitable_gpu = nullptr;

    for(auto& device : physical_devices) {
        if(!is_device_suitable(&device)) continue;

        if(device.get_physical_properties()->deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            return device;
        }

        any_suitable_gpu = &device;
    }

    if(!any_suitable_gpu) {
        throw std::runtime_error("could not find any suitable GPU");
    }

    return *any_suitable_gpu;
}

bool Application::is_device_suitable(const VK::PhysicalDevice* device) {

    if(device->get_queue_family_indices()->find_family(VK_QUEUE_GRAPHICS_BIT) < 0) return false;
    if(device->get_queue_family_indices()->find_surface_present_family(m_surface) < 0) return false;
    if(!device->supports_extensions(m_device_extensions)) return false;
    if(!VK::SwapChainSupportDetails(device, m_surface.unowned_copy()).is_complete()) return false;
    if(!device->get_physical_features()->samplerAnisotropy) return false;

    return true;
}

bool Application::check_validation_layer_support() {
    bool result = true;

    auto available_layers = m_instance.get_validation_layers();

    std::cout << "Supported validation layers:\n";

    for(auto& layer : available_layers) {
        std::cout << "\t" << layer.layerName << " ver. " << VK::VersionCode(layer.specVersion) << " impl. " << VK::VersionCode(layer.implementationVersion) << ": " << layer.description << "\n";
    }

    for (const char* required_layer : m_required_validation_layers) {
        auto layer = std::find_if(available_layers.begin(), available_layers.end(), [required_layer](auto available_layer) -> bool {
            return strcmp(required_layer, available_layer.layerName) == 0;
        });

        if(layer == available_layers.end()) {
            result = false;
            std::cout << "Warning: Validation layer '" << required_layer << "' is required but not available\n";
        }
    }

    return result;
}

void Application::main_loop() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwWaitEventsTimeout(1.0 / 60.0);

        draw_frame();
    }
    m_device.wait_idle();
}

void Application::cleanup() {
    if(!m_window) return;

    cleanup_swap_chain();

    m_image_available_semaphores.clear();
    m_render_finished_semaphores.clear();
    m_in_flight_fences.clear();

    m_texture_image_view.destroy();
    if(m_texture_sampler) m_texture_sampler.destroy();
    if(m_texture_image) m_texture_image->destroy();

    m_descriptor_set_layout.destroy();

    if(m_index_buffer) m_index_buffer->destroy();
    if(m_vertex_buffer) m_vertex_buffer->destroy();

    m_debug_callback_handler.stop_listening();

    m_swapchain.destroy();
    m_physical_device = nullptr;

    m_surface.destroy();
    m_instance.destroy();

    glfwDestroyWindow(m_window);
    glfwTerminate();

    m_window = nullptr;
}

static void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
    auto app = (Application*) glfwGetWindowUserPointer(window);
    app->m_framebuffer_resized = true;
}

void Application::init_window() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(m_window_width, m_window_height, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_resize_callback);
}

void Application::create_logical_device() {

    m_physical_device = std::make_unique<VK::PhysicalDevice>(get_best_physical_device());
    auto queue_family_indices = m_physical_device->get_queue_family_indices();

    m_graphics_queue_family = queue_family_indices->find_family(VK_QUEUE_GRAPHICS_BIT);
    m_present_queue_family = queue_family_indices->find_surface_present_family(m_surface);

    VK::DeviceFactory device_factory;

    if(m_enable_validation_layers) {
        device_factory.get_validation_layers() = m_required_validation_layers;
    }
    device_factory.get_enabled_extensions() = m_device_extensions;

    float queue_priorities[] = { 1.0f };
    device_factory.add_queue(0, m_graphics_queue_family, queue_priorities);
    if(m_graphics_queue_family != m_present_queue_family) {
        device_factory.add_queue(0, m_present_queue_family, queue_priorities);
    }

    device_factory.get_device_features().samplerAnisotropy = VK_TRUE;

    m_device = device_factory.create(m_physical_device.get());

    m_device_graphics_queue = m_device.get_queue(m_graphics_queue_family, 0);
    m_device_present_queue = m_device.get_queue(m_present_queue_family, 0);

    m_msaa_samples = m_physical_device->get_max_usable_sample_count();

    m_command_pool = VK::CommandPool::create(
        &m_device,
        m_graphics_queue_family,
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
    );
}

void Application::create_swap_chain() {

    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);

    VK::SwapChainSupportDetails swap_chain_support(m_physical_device.get(), m_surface.unowned_copy());

    VkSurfaceFormatKHR surface_format = swap_chain_support.choose_best_format();
    m_swapchain_extent = swap_chain_support.choose_best_swap_extent(width, height);
    uint32_t image_count = swap_chain_support.get_optimal_chain_image_count();

    m_swapchain_image_format = surface_format.format;

    VK::SwapchainFactory factory;
    factory.set_surface(m_surface);
    factory.set_min_image_count(image_count);
    factory.set_image_format(surface_format.format);
    factory.set_image_color_space(surface_format.colorSpace);
    factory.set_image_extent(m_swapchain_extent);
    factory.set_image_usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    factory.set_pre_transform(swap_chain_support.m_capabilities.currentTransform);
    factory.set_composite_alpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR);
    factory.set_clipped(VK_TRUE);

    if (m_graphics_queue_family != m_present_queue_family) {
        factory.set_image_sharing_mode(VK_SHARING_MODE_CONCURRENT);
        factory.get_queue_family_indices().assign({(uint32_t)m_graphics_queue_family, (uint32_t)m_present_queue_family});
    } else {
        factory.set_image_sharing_mode(VK_SHARING_MODE_EXCLUSIVE);
    }

    m_swapchain = factory.create(&m_device);
}

void Application::create_graphics_pipeline() {

    VK::PipelineFactory pipeline_factory {};

    auto vertex_shader = VK::ShaderModule::from_file(&m_device, "resources/shaders/vert.spv");
    auto fragment_shader = VK::ShaderModule::from_file(&m_device, "resources/shaders/frag.spv");

    pipeline_factory.shader_stages.add_shader(vertex_shader, VK_SHADER_STAGE_VERTEX_BIT);
    pipeline_factory.shader_stages.add_shader(fragment_shader, VK_SHADER_STAGE_FRAGMENT_BIT);

    auto vertex_array_binding = pipeline_factory.input_vertex_state.create_binding(0, 8 * sizeof(float));
    vertex_array_binding.add_attribute(VK_FORMAT_R32G32B32_SFLOAT, 0, sizeof(float) * 0);
    vertex_array_binding.add_attribute(VK_FORMAT_R32G32B32_SFLOAT, 1, sizeof(float) * 3);
    vertex_array_binding.add_attribute(VK_FORMAT_R32G32_SFLOAT, 2, sizeof(float) * 6);

    pipeline_factory.viewport_state.add_viewport(VK::Viewport(m_swapchain_extent));
    pipeline_factory.viewport_state.add_scissor(VkRect2D {{0, 0}, m_swapchain_extent});

    pipeline_factory.rasterization_state.set_cull_mode(VK_CULL_MODE_BACK_BIT);
    pipeline_factory.rasterization_state.set_front_face(VK_FRONT_FACE_COUNTER_CLOCKWISE);

    pipeline_factory.multisampling_state.set_rasterization_samples(m_msaa_samples);

    VK::PipelineColorAttachmentState pipeline_color_attachment_states {};

    pipeline_factory.color_blend_state_create_info.add_color_attachment(pipeline_color_attachment_states);

    pipeline_factory.depth_stencil_states.set_depth_test_enable(true);
    pipeline_factory.depth_stencil_states.set_depth_write_enable(true);
    pipeline_factory.depth_stencil_states.set_depth_compare_op(VK_COMPARE_OP_LESS);

    VkDescriptorSetLayout descriptors[] { m_descriptor_set_layout.get_handle() };

    m_pipeline_layout = VK::PipelineLayout::create(&m_device, descriptors, {});
    m_graphics_pipeline = pipeline_factory.create(m_pipeline_layout, m_render_pass);
}

void Application::create_render_pass() {

    VK::Attachment color_attachment { m_swapchain_image_format };
    color_attachment.set_load_store_operations(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE);
    color_attachment.set_samples(m_msaa_samples);
    color_attachment.set_final_layout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    VK::Attachment depth_attachment { find_depth_format() };
    depth_attachment.set_load_store_operations(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE);
    depth_attachment.set_samples(m_msaa_samples);
    depth_attachment.set_final_layout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    VK::Attachment resolve_attachment { m_swapchain_image_format };
    resolve_attachment.set_samples(VK_SAMPLE_COUNT_1_BIT);
    resolve_attachment.set_load_store_operations(VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE);
    resolve_attachment.set_final_layout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    VkAttachmentReference color_attachment_ref { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkAttachmentReference depth_attachment_ref { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
    VkAttachmentReference color_resolve_attachment_ref { 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

    VkSubpassDescription subpass {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;
    subpass.pResolveAttachments = &color_resolve_attachment_ref;

    VkSubpassDependency dependency {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcAccessMask = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VK::RenderPassFactory render_pass_factory;
    render_pass_factory.get_attachment_descriptions().assign({
         color_attachment.get_description(), depth_attachment.get_description(), resolve_attachment.get_description()
    });
    render_pass_factory.get_subpass_descriptions().assign({ subpass });
    render_pass_factory.get_subpass_dependency_descriptions().assign({ dependency });
    m_render_pass = render_pass_factory.create(&m_device);
}

void Application::create_swapchain_images() {
    auto swapchain_images = m_swapchain.get_swapchain_images();

    for(auto& swapchain_image : swapchain_images) {
        m_swapchain_images.emplace_back();
        auto& image = m_swapchain_images.back();

        VK::ImageViewFactory image_view_factory;
        image_view_factory.set_format(m_swapchain_image_format);
        image_view_factory.get_subresource_range().aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image.m_image_view = image_view_factory.create(&m_device, swapchain_image);

        VK::FramebufferFactory framebuffer_factory;
        framebuffer_factory.set_size(m_swapchain_extent);
        framebuffer_factory.get_attachments().assign({
                                                             m_color_image_view.unowned_copy(),
                                                             m_depth_image_view.unowned_copy(),
                                                             image.m_image_view.unowned_copy()
        });

        image.m_framebuffer = framebuffer_factory.create(m_render_pass);
    }
}

void Application::create_command_buffers() {

    auto& command_pool = m_command_pool;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        m_command_buffers.push_back(command_pool.create_command_buffer());
    }
}

void Application::record_command_buffer(uint32_t frame_index, uint32_t swapchain_frame_index) {

    VK::CommandBuffer& command_buffer = m_command_buffers[frame_index];
    VK::SwapchainEntry& swapchain_entry = m_swapchain_images[swapchain_frame_index];

    command_buffer.reset();
    command_buffer.begin(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    VkRenderPassBeginInfo render_pass_begin_info {};
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.renderPass = m_render_pass.get_handle();
    render_pass_begin_info.framebuffer = swapchain_entry.m_framebuffer.get_handle();

    render_pass_begin_info.renderArea.offset = {0, 0};
    render_pass_begin_info.renderArea.extent = m_swapchain_extent;

    VkClearValue clear_values[2] {};
    clear_values[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clear_values[1].depthStencil = {1.0f, 0};

    render_pass_begin_info.clearValueCount = 2;
    render_pass_begin_info.pClearValues = clear_values;

    command_buffer.begin_render_pass(&render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    command_buffer.bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);

    VkDescriptorSet descriptors[] = {m_descriptor_set_array->get_descriptor_sets()[frame_index] };
    VkBuffer vertex_buffers[] = { m_vertex_buffer->get_buffer().get_handle() };
    VkDeviceSize offsets[] = { 0 };

    command_buffer.bind_vertex_buffers(vertex_buffers, offsets);
    command_buffer.bind_index_buffer(m_index_buffer->get_buffer(), 0, VK_INDEX_TYPE_UINT32);
    command_buffer.bind_descriptor_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline_layout.get_handle(), descriptors, {});
    command_buffer.draw_indexed(m_index_buffer_storage.size(), 1, 0, 0, 0);
    command_buffer.end_render_pass();

    command_buffer.end();
}

void Application::create_sync_objects() {

    m_image_available_semaphores.reserve(MAX_FRAMES_IN_FLIGHT);
    m_render_finished_semaphores.reserve(MAX_FRAMES_IN_FLIGHT);
    m_in_flight_fences.reserve(MAX_FRAMES_IN_FLIGHT);
    m_in_flight_images.resize(m_swapchain_images.size(), {nullptr, nullptr });

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        m_image_available_semaphores.push_back(VK::Semaphore::create(&m_device));
        m_render_finished_semaphores.push_back(VK::Semaphore::create(&m_device));
        m_in_flight_fences.push_back(VK::Fence::create(&m_device, VK_FENCE_CREATE_SIGNALED_BIT));
    }
}

void Application::draw_frame() {

    m_in_flight_fences[m_current_frame].wait_one();

    uint32_t image_index = 0;
    auto result = m_swapchain.acquire_next_image(&image_index, m_image_available_semaphores[m_current_frame]);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        m_framebuffer_resized = false;
        recreate_swap_chain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (m_in_flight_images[image_index]) {
        m_in_flight_images[image_index].wait_one();
    }
    // Mark the image as now being in use by this frame
    m_in_flight_images[image_index] = m_in_flight_fences[m_current_frame].unowned_copy();

    update_uniform_buffer(m_current_frame);

    m_in_flight_fences[m_current_frame].reset_one();

    auto& swapchain_entry = m_swapchain_images[image_index];
    auto& command_buffer = m_command_buffers[m_current_frame];

    record_command_buffer(m_current_frame, image_index);

    VkSemaphore signal_semaphores[] = { m_render_finished_semaphores[m_current_frame].get_handle() };
    VkSemaphore wait_semaphores[] = { m_image_available_semaphores[m_current_frame].get_handle() };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    command_buffer.submit(
            m_device_graphics_queue,
            m_in_flight_fences[m_current_frame].get_handle(),
            signal_semaphores, wait_semaphores, wait_stages);

    VkSwapchainKHR swap_chains[] = { m_swapchain.get_handle() };

    VkPresentInfoKHR present_info {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swap_chains;
    present_info.pImageIndices = &image_index;
    present_info.pResults = nullptr; // Optional

    result = vkQueuePresentKHR(m_device_graphics_queue, &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebuffer_resized) {
        m_framebuffer_resized = false;
        recreate_swap_chain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    vkQueueWaitIdle(m_device_present_queue);

    m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Application::cleanup_swap_chain() {

    m_color_image_view.destroy();
    m_depth_image_view.destroy();

    if(m_color_image) m_color_image->destroy();
    if(m_depth_image) m_depth_image->destroy();

    m_command_buffers.clear();
    m_uniform_buffers.clear();

    m_descriptor_pool.destroy();
    m_graphics_pipeline.destroy();
    m_pipeline_layout.destroy();
    m_render_pass.destroy();
    m_swapchain_images.clear();
}

void Application::recreate_swap_chain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(m_window, &width, &height);
        glfwWaitEvents();
    }

    m_device.wait_idle();

    cleanup_swap_chain();

    create_swap_chain();
    create_color_resources();
    create_depth_resources();
    create_render_pass();
    create_swapchain_images();
    create_graphics_pipeline();
    create_uniform_buffers();
    create_descriptor_pool();
    create_descriptor_sets();
    create_command_buffers();
}

void Application::create_mesh() {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::string model_path = "resources/models/viking_room.obj";

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, model_path.c_str())) {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {

            float vertex[] = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2],
                1.0f, 1.0f, 1.0f,
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
            };

            for(auto num : vertex) {
                m_vertex_buffer_storage.push_back(num);
            }

            m_index_buffer_storage.push_back(m_index_buffer_storage.size());
        }
    }
}

void Application::create_index_buffer() {

    auto index_buffer_size = m_index_buffer_storage.size() * sizeof(m_index_buffer_storage[0]);
    auto staging_buffer = VK::StagingBufferFactory().create_staging_buffer(&m_device, m_index_buffer_storage);

    VK::BufferFactory index_buffer_factory {};
    index_buffer_factory.set_size(index_buffer_size);
    index_buffer_factory.set_usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    index_buffer_factory.set_memory_properties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_index_buffer = std::make_unique<VK::MemoryBuffer>(index_buffer_factory.create_memory_buffer(&m_device));

    auto command_buffer = m_command_pool.create_command_buffer();
    command_buffer.begin(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    auto copy_command = VK::CopyBufferCommand(&staging_buffer.get_buffer(), &m_index_buffer->get_buffer());
    copy_command.set_size(index_buffer_size);
    copy_command.write(&command_buffer);

    command_buffer.end();
    command_buffer.submit_and_wait(m_device_graphics_queue, nullptr);
}

void Application::create_vertex_buffer() {

    auto vertex_buffer_size = m_vertex_buffer_storage.size() * sizeof(m_vertex_buffer_storage[0]);
    auto staging_buffer = VK::StagingBufferFactory().create_staging_buffer(&m_device, m_vertex_buffer_storage);

    VK::BufferFactory vertex_buffer_factory {};
    vertex_buffer_factory.set_memory_properties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vertex_buffer_factory.set_usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    vertex_buffer_factory.set_size(vertex_buffer_size);
    m_vertex_buffer = std::make_unique<VK::MemoryBuffer>(vertex_buffer_factory.create_memory_buffer(&m_device));

    auto command_buffer = m_command_pool.create_command_buffer();
    command_buffer.begin(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    auto copy_command = VK::CopyBufferCommand(&staging_buffer.get_buffer(), &m_vertex_buffer->get_buffer());
    copy_command.set_size(vertex_buffer_size);
    copy_command.write(&command_buffer);

    command_buffer.end();
    command_buffer.submit_and_wait(m_device_graphics_queue, nullptr);
}

void Application::create_uniform_buffers() {
    VkDeviceSize buffer_size = sizeof(UniformBufferObject);

    m_uniform_buffers.reserve(MAX_FRAMES_IN_FLIGHT);

    VK::BufferFactory factory {};

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        factory.set_memory_properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        factory.set_usage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        factory.set_size(buffer_size);
        m_uniform_buffers.push_back(factory.create_memory_buffer(&m_device));
    }
}

void Application::update_uniform_buffer(uint32_t image_index) {

    float dt = 0.01;
    float camera_speed = 1.0f;

    glm::vec3 camera_dir { m_camera_direction.x, 0, m_camera_direction.z };

    float length = sqrt(camera_dir.x * camera_dir.x + camera_dir.y * camera_dir.y + camera_dir.z * camera_dir.z);

    camera_dir /= length;

    glm::vec3 camera_side { camera_dir.z, 0, -camera_dir.x };

    if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) m_camera_pos += camera_dir * dt * camera_speed;
    if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) m_camera_pos -= camera_dir * dt * camera_speed;
    if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) m_camera_pos += camera_side * dt * camera_speed;
    if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) m_camera_pos -= camera_side * dt * camera_speed;

    if(glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) m_camera_pos.y += dt * camera_speed;
    if(glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) m_camera_pos.y -= dt * camera_speed;

    if(glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) m_pitch += dt;
    if(glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) m_pitch -= dt;
    if(glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) m_yaw -= dt;
    if(glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) m_yaw += dt;

    glm::mat4x4 camera_transform { 1 };
    camera_transform = glm::rotate(camera_transform, m_pitch, glm::vec3 {1, 0, 0});
    camera_transform = glm::rotate(camera_transform, m_yaw, glm::vec3 {0, 1, 0});
    glm::vec4 new_camera_dir = (glm::vec4 {0, 0, 1, 0} * camera_transform);
    m_camera_direction = { new_camera_dir.x, new_camera_dir.y, new_camera_dir.z };

    UniformBufferObject ubo {};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::rotate(ubo.model, glm::radians(90.f), glm::vec3(0.0, 1.0, 0.0));
    ubo.model = glm::scale(ubo.model, glm::vec3(0.05, 0.05, 0.05));
    ubo.view = glm::lookAt(m_camera_pos, m_camera_pos + m_camera_direction, glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), (float) m_swapchain_extent.width / (float) m_swapchain_extent.height, 0.01f, 100.0f);

    ubo.proj[1][1] *= -1;

    m_uniform_buffers[image_index].get_memory().set_data(&ubo, sizeof(ubo));
}

void Application::create_descriptor_set_layout() {
    VK::DescriptorSetLayoutFactory factory;

    VK::DescriptorSetLayoutBinding ubo_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    ubo_layout_binding.set_stage_flags(VK_SHADER_STAGE_VERTEX_BIT);

    VK::DescriptorSetLayoutBinding sampler_layout_binding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
    sampler_layout_binding.set_stage_flags(VK_SHADER_STAGE_FRAGMENT_BIT);

    factory.bind_descriptor(0, ubo_layout_binding);
    factory.bind_descriptor(1, sampler_layout_binding);

    m_descriptor_set_layout = factory.create(&m_device);
}

void Application::create_descriptor_pool() {
    VK::DescriptorPoolFactory factory;
    factory.add_pool_size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT);
    factory.add_pool_size(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT);
    factory.set_max_sets(MAX_FRAMES_IN_FLIGHT);

    m_descriptor_pool = factory.create(&m_device);
}

void Application::create_descriptor_sets() {
    m_descriptor_set_array = std::make_unique<VK::DescriptorSetArray>(m_descriptor_pool.unowned_copy());
    m_descriptor_set_array->get_layouts().resize(MAX_FRAMES_IN_FLIGHT, m_descriptor_set_layout.get_handle());
    m_descriptor_set_array->create();

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

        VK::BufferDescriptor uniform_buffer_descriptor(m_uniform_buffers[i].get_buffer(), 0, sizeof(UniformBufferObject));
        VK::SamplerDescriptor texture_sampler_descriptor(m_texture_sampler, m_texture_image_view);

        m_descriptor_set_array->bind_descriptor(i, 0, uniform_buffer_descriptor);
        m_descriptor_set_array->bind_descriptor(i, 1, texture_sampler_descriptor);
    }
}

void Application::create_texture_image() {
    const char* path = "resources/models/viking_room.png";
    FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(path, 0), path);
    FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    auto* image = (unsigned char*) FreeImage_GetBits(converted);
    auto image_width = (uint32_t)FreeImage_GetWidth(converted);
    auto image_height = (uint32_t)FreeImage_GetHeight(converted);
    auto image_size = image_width * image_height * 4;

    m_mip_levels = (int) floor(log2(std::max(image_width, image_height))) + 1;

    for (int pix = 0; pix < image_size; pix += 4){
        std::swap(image[pix + 0], image[pix + 2]);
    }

    auto staging_buffer = VK::StagingBufferFactory().create_staging_buffer(&m_device, image, image_size);

    FreeImage_Unload(converted);

    VK::ImageFactory image_factory;
    image_factory.set_extent({image_width, image_height, 1});
    image_factory.set_mip_levels(m_mip_levels);
    image_factory.set_usage(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    image_factory.set_format(VK_FORMAT_R8G8B8A8_SRGB);

    m_texture_image = std::make_unique<VK::MemoryImage>(image_factory.create(&m_device));

    auto command_buffer = m_command_pool.create_command_buffer();
    command_buffer.begin(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    VK::ImageMemoryBarrier layout_conversion_barrier { &m_texture_image->get_image() };
    layout_conversion_barrier.set_layouts(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    layout_conversion_barrier.set_mip_level_count(m_mip_levels);
    layout_conversion_barrier.set_access_masks(0, VK_ACCESS_TRANSFER_WRITE_BIT);
    layout_conversion_barrier.set_aspect_mask(VK_IMAGE_ASPECT_COLOR_BIT);
    layout_conversion_barrier.write(&command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

    auto copy_command = VK::CopyBufferToImageCommand(&staging_buffer.get_buffer(), &m_texture_image->get_image());
    copy_command.set_image_extent(image_factory.get_extent());
    copy_command.set_destination_image_layout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copy_command.write(&command_buffer);

    generate_mipmaps(&command_buffer, &m_texture_image->get_image(), image_factory.get_format(), { image_width, image_height }, image_factory.get_mip_levels());

    command_buffer.end();
    command_buffer.submit_and_wait(m_device_graphics_queue, nullptr);

    VK::ImageViewFactory image_view_factory;
    image_view_factory.set_format(image_factory.get_format());
    image_view_factory.get_subresource_range().aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_texture_image_view = image_view_factory.create(&m_device, m_texture_image->get_image());
}

void Application::generate_mipmaps(VK::CommandBuffer* command_buffer, VK::Image* image, VkFormat format, VkExtent2D extent, int mip_levels) {

    VkFormatProperties format_properties {};
    m_physical_device->get_format_properties(&format_properties, format);

    if (!(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        throw std::runtime_error("texture image format does not support linear blitting");
    }

    auto mip_width = (int32_t) extent.width;
    auto mip_height = (int32_t) extent.height;

    VK::ImageMemoryBarrier barrier { image };
    barrier.set_aspect_mask(VK_IMAGE_ASPECT_COLOR_BIT);

    for (uint32_t i = 1; i < mip_levels; i++) {

        barrier.set_mip_level_base(i - 1);
        barrier.set_layouts(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        barrier.set_access_masks(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
        barrier.write(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

        VK::ImageBlitCommand blit_command(image, image);
        blit_command.set_source_layout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        blit_command.set_destination_layout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        blit_command.set_source_mip_level(i - 1);
        blit_command.set_destination_mip_level(i);
        blit_command.setup_mip_offsets(mip_width, mip_height);
        blit_command.write(command_buffer);

        barrier.set_layouts(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        barrier.set_access_masks(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT);
        barrier.write(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

        if (mip_width > 1) mip_width /= 2;
        if (mip_height > 1) mip_height /= 2;
    }

    barrier.set_mip_level_base(mip_levels - 1);
    barrier.set_layouts(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    barrier.set_access_masks(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    barrier.write(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
}

void Application::create_texture_sampler() {
    VK::SamplerFactory sampler_factory {};
    sampler_factory.set_mag_filter(VK_FILTER_LINEAR);
    sampler_factory.set_min_filter(VK_FILTER_LINEAR);
    sampler_factory.set_address_modes_uvw(VK_SAMPLER_ADDRESS_MODE_REPEAT);
    sampler_factory.set_anisotropy_enable(VK_TRUE);
    sampler_factory.set_max_anisotropy(m_physical_device->get_physical_properties()->limits.maxSamplerAnisotropy);
    sampler_factory.set_max_lod((float) m_mip_levels);
    sampler_factory.set_min_lod((float) 0.0f);

    m_texture_sampler = sampler_factory.create(&m_device);
}

VkFormat Application::find_depth_format() {
    return m_physical_device->find_supported_format(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

void Application::create_depth_resources() {
    VkFormat depth_format = find_depth_format();

    VK::ImageFactory image_factory;
    image_factory.set_samples(m_msaa_samples);
    image_factory.set_extent({m_swapchain_extent.width, m_swapchain_extent.height, 1 });
    image_factory.set_usage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    image_factory.set_format(depth_format);

    m_depth_image = std::make_unique<VK::MemoryImage>(image_factory.create(&m_device));

    VK::ImageViewFactory image_view_factory;
    image_view_factory.set_format(depth_format);
    image_view_factory.get_subresource_range().aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    m_depth_image_view = image_view_factory.create(&m_device, m_depth_image->get_image());

    auto command_buffer = m_command_pool.create_command_buffer();
    command_buffer.begin(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    VK::ImageMemoryBarrier layout_conversion_barrier { &m_depth_image->get_image() };
    layout_conversion_barrier.set_layouts(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    layout_conversion_barrier.set_access_masks(0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    layout_conversion_barrier.set_aspect_mask(VK_IMAGE_ASPECT_DEPTH_BIT);
    layout_conversion_barrier.write(&command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);

    command_buffer.end();
    command_buffer.submit_and_wait(m_device_graphics_queue, nullptr);
}

void Application::create_color_resources() {
    VkFormat color_format = m_swapchain_image_format;

    VK::ImageFactory image_factory;
    image_factory.set_extent({m_swapchain_extent.width, m_swapchain_extent.height, 1});
    image_factory.set_samples(m_msaa_samples);
    image_factory.set_usage(VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    image_factory.set_format(color_format);
    m_color_image = std::make_unique<VK::MemoryImage>(image_factory.create(&m_device));

    VK::ImageViewFactory image_view_factory;
    image_view_factory.set_format(color_format);
    image_view_factory.get_subresource_range().aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_color_image_view = image_view_factory.create(&m_device, m_color_image->get_image());
}