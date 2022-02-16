//
// Created by Артем on 09.02.2022.
//

#include "app.hpp"
#include "vulkan/codes/vk-physical-device-type-code.hpp"
#include "vulkan/vk-swap-chain-support-details.hpp"
#include "shader-reader.hpp"
#include <FreeImage.h>
#include <set>
#include <random>
#include <chrono>
#include <thread>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "vulkan/vk-buffer.hpp"
#include "vulkan/commands/vk-copy-buffer-command.hpp"
#include "vulkan/commands/copy-buffer-to-image-command.hpp"

void HelloTriangleApplication::create_instance() {
    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &appInfo;

    std::vector<const char*> required_extensions = get_required_extensions();

    create_info.enabledExtensionCount = required_extensions.size();
    create_info.ppEnabledExtensionNames = required_extensions.data();

    if (m_enable_validation_layers) {
        create_info.enabledLayerCount = static_cast<uint32_t>(m_required_validation_layers.size());
        create_info.ppEnabledLayerNames = m_required_validation_layers.data();
    } else {
        create_info.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&create_info, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> extensions(extension_count);

    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    std::cout << "Supported extensions:\n";
    for(auto& ext : extensions) {
        std::cout << "\t" << ext.extensionName << " ver. " << VK::VersionCode(ext.specVersion) << "\n";
    }
}

std::vector<const char*> HelloTriangleApplication::get_required_extensions() const {
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;

    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::cout << "Required GLFW extensions:\n";
    for(int i = 0; i < glfw_extension_count; i++) {
        std::cout << "\t" << glfw_extensions[i] << "\n";
    }

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if(m_enable_validation_layers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void HelloTriangleApplication::init_vulkan() {
    setup_validation_layers();
    create_instance();
    setup_debug_messenger();
    create_surface();
    create_logical_device();
    create_swap_chain();
    create_image_views();
    create_render_pass();
    create_descriptor_set_layout();
    create_graphics_pipeline();
    create_command_pool();
    create_mesh();
    create_index_buffer();
    create_vertex_buffer();
    create_color_resources();
    create_depth_resources();
    create_framebuffers();
    create_texture_image();
    create_texture_sampler();
    create_uniform_buffers();
    create_descriptor_pool();
    create_descriptor_sets();
    create_sync_objects();
    create_command_buffers();
}

void HelloTriangleApplication::create_surface() {
    if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void HelloTriangleApplication::setup_validation_layers() {
    if (m_enable_validation_layers && !check_validation_layer_support()) {
        throw std::runtime_error("some requested validation layers are not available");
    }
}

void HelloTriangleApplication::setup_debug_messenger() {
    if(m_enable_debug_messenger && !m_debug_callback_handler.listen(m_instance)) {
        throw std::runtime_error("debug callback handler failed to initialize");
    }
}

VK::PhysicalDevice HelloTriangleApplication::get_best_physical_device() {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);
    if(device_count == 0) {
        throw std::runtime_error("could not find any Vulkan-compatible GPU");
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

    std::vector<VK::PhysicalDevice> physical_devices {};
    physical_devices.reserve(devices.size());

    for(auto& device : devices) {
        physical_devices.emplace_back(device);
    }

    return *select_best_physical_device(physical_devices);
}

const VK::PhysicalDevice* HelloTriangleApplication::select_best_physical_device(const std::vector<VK::PhysicalDevice>& devices) {
    const VK::PhysicalDevice* discrete_gpu = nullptr;
    const VK::PhysicalDevice* any_suitable_gpu = nullptr;

    for(auto& device : devices) {

        if(!is_device_suitable(&device)) continue;

        if(device.get_physical_properties()->deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            discrete_gpu = &device;
            break;
        } else {
            any_suitable_gpu = &device;
        }
    }

    if(!discrete_gpu && !any_suitable_gpu) {
        throw std::runtime_error("could not find any suitable GPU");
    }

    if(discrete_gpu) return discrete_gpu;
    return any_suitable_gpu;
}

bool HelloTriangleApplication::is_device_suitable(const VK::PhysicalDevice* device) {

    auto graphics_family = device->get_queue_family_indices()->find_family(VK_QUEUE_GRAPHICS_BIT);
    if(graphics_family < 0) return false;

    auto surface_family = device->get_queue_family_indices()->find_surface_present_family(m_surface);
    if(surface_family < 0) return false;

    if(!device->supports_extensions(m_device_extensions)) return false;
    if(!VK::SwapChainSupportDetails(device, m_surface).is_complete()) return false;

    if(!device->get_physical_features()->samplerAnisotropy) return false;

    return true;
}

bool HelloTriangleApplication::check_validation_layer_support() {
    bool result = true;

    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    std::cout << "Supported validation layers:\n";

    for(auto& layer : available_layers) {
        std::cout << "\t" << layer.layerName << " ver. " << VK::VersionCode(layer.specVersion) << " impl. " << VK::VersionCode(layer.implementationVersion) << ": " << layer.description << "\n";
    }

    for (const char* required_layer : m_required_validation_layers) {
        bool layerFound = false;

        for (const auto& available_layer : available_layers) {
            if (strcmp(required_layer, available_layer.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            result = false;
            std::cout << "Warning: Validation layer '" << required_layer << "' is required but not available\n";
        }
    }

    return result;
}

void HelloTriangleApplication::main_loop() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwWaitEventsTimeout(1.0 / 60.0);

        draw_frame();
    }
    vkDeviceWaitIdle(m_surface_context->get_device()->get_handle());
}

void HelloTriangleApplication::cleanup() {
    if(!m_window) return;

    cleanup_swap_chain();

    if(m_texture_sampler) {
        vkDestroySampler(m_surface_context->get_device()->get_handle(), m_texture_sampler, nullptr);
        m_texture_sampler = nullptr;
    }

    if(m_texture_image_view) m_texture_image_view->destroy();
    if(m_texture_image) m_texture_image->destroy();
    m_texture_image_memory.free();

    if(m_descriptor_set_layout) {
        vkDestroyDescriptorSetLayout(m_surface_context->get_device()->get_handle(), m_descriptor_set_layout, nullptr);
        m_descriptor_set_layout = nullptr;
    }

    if(m_index_buffer) m_index_buffer->destroy();
    m_index_buffer_memory.free();

    if(m_vertex_buffer) m_vertex_buffer->destroy();
    m_vertex_buffer_memory.free();

    m_debug_callback_handler.stop_listening();

    for(auto& fence : m_in_flight_fences) {
        vkDestroyFence(m_surface_context->get_device()->get_handle(), fence, nullptr);
    }
    m_in_flight_fences.clear();

    for (auto& semaphore : m_image_available_semaphores) {
        vkDestroySemaphore(m_surface_context->get_device()->get_handle(), semaphore, nullptr);
    }
    m_image_available_semaphores.clear();

    for (auto& semaphore : m_render_finished_semaphores) {
        vkDestroySemaphore(m_surface_context->get_device()->get_handle(), semaphore, nullptr);
    }
    m_render_finished_semaphores.clear();

    m_command_buffers.clear();
    m_surface_context = nullptr;
    m_physical_device = nullptr;

    if(m_surface) {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        m_surface = nullptr;
    }

    vkDestroyInstance(m_instance, nullptr);

    glfwDestroyWindow(m_window);
    glfwTerminate();

    m_window = nullptr;
}

static void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
    auto app = (HelloTriangleApplication*) glfwGetWindowUserPointer(window);
    app->m_framebuffer_resized = true;
}

void HelloTriangleApplication::init_window() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(m_window_width, m_window_height, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_resize_callback);
}

void HelloTriangleApplication::create_logical_device() {

    m_physical_device = std::make_unique<VK::PhysicalDevice>(get_best_physical_device());
    m_surface_context = std::make_unique<VK::SurfaceContext>(m_physical_device.get(), m_surface);

    if(m_enable_validation_layers) {
        m_surface_context->create_logical_device(m_device_extensions, m_required_validation_layers);
    } else {
        m_surface_context->create_logical_device(m_device_extensions, {});
    }

    m_msaa_samples = m_physical_device->get_max_usable_sample_count();
}

void HelloTriangleApplication::create_swap_chain() {

    VK::SwapChainSupportDetails swap_chain_support(m_physical_device.get(), m_surface);

    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);

    VkSurfaceFormatKHR surface_format = swap_chain_support.choose_best_format();
    VkPresentModeKHR present_mode = swap_chain_support.choose_best_present_mode();
    VkExtent2D extent = swap_chain_support.choose_best_swap_extent(width, height);

    uint32_t image_count = swap_chain_support.get_optimal_chain_image_count();

    VkSwapchainCreateInfoKHR create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = m_surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queue_family_indices[] = {
        (uint32_t) m_surface_context->get_graphics_queue_index(),
        (uint32_t) m_surface_context->get_present_queue_index()
    };

    if (m_surface_context->get_graphics_queue_index() != m_surface_context->get_present_queue_index()) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = nullptr;
    }

    create_info.preTransform = swap_chain_support.m_capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.clipped = VK_TRUE;

    create_info.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_surface_context->get_device()->get_handle(), &create_info, nullptr, &m_swap_chain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain");
    }

    vkGetSwapchainImagesKHR(m_surface_context->get_device()->get_handle(), m_swap_chain, &image_count, nullptr);
    m_swap_chain_images.resize(image_count);
    vkGetSwapchainImagesKHR(m_surface_context->get_device()->get_handle(), m_swap_chain, &image_count, m_swap_chain_images.data());

    m_swap_chain_image_format = surface_format.format;
    m_swap_chain_extent = extent;
}

void HelloTriangleApplication::create_image_views() {
    m_swap_chain_image_views.reserve(m_swap_chain_images.size());

    for (auto& m_swap_chain_image : m_swap_chain_images) {
        m_swap_chain_image_views.push_back(std::make_unique<VK::ImageView>(m_swap_chain_image, m_surface_context->get_device()));
        auto& image_view = m_swap_chain_image_views.back();

        image_view->get_subresource_range().aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view->set_format(m_swap_chain_image_format);
        image_view->set_view_type(VK_IMAGE_VIEW_TYPE_2D);
        image_view->create();
    }
}

void HelloTriangleApplication::create_graphics_pipeline() {
    auto vert_shader_code = read_file("resources/shaders/vert.spv");
    auto frag_shader_code = read_file("resources/shaders/frag.spv");

    auto vert_shader_module = create_shader_module(vert_shader_code);
    auto frag_shader_module = create_shader_module(frag_shader_code);

    VkPipelineShaderStageCreateInfo vert_shader_stage_info {};
    vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_stage_info.module = vert_shader_module;
    vert_shader_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo frag_shader_stage_info {};
    frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_info.module = frag_shader_module;
    frag_shader_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo shader_stages[] = {
        vert_shader_stage_info,
        frag_shader_stage_info
    };

    VkVertexInputBindingDescription binding_description {};
    binding_description.binding = 0;
    binding_description.stride = 8 * sizeof(float);
    binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription attribute_descriptions[3] {};

    attribute_descriptions[0].binding = 0;
    attribute_descriptions[0].location = 0;
    attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute_descriptions[0].offset = sizeof(float) * 0;

    attribute_descriptions[1].binding = 0;
    attribute_descriptions[1].location = 1;
    attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute_descriptions[1].offset = sizeof(float) * 3;

    attribute_descriptions[2].binding = 0;
    attribute_descriptions[2].location = 2;
    attribute_descriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attribute_descriptions[2].offset = sizeof(float) * 6;

    VkPipelineVertexInputStateCreateInfo vertex_input_info {};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount = 0;
    vertex_input_info.pVertexBindingDescriptions = nullptr; // Optional
    vertex_input_info.vertexAttributeDescriptionCount = 0;
    vertex_input_info.pVertexAttributeDescriptions = nullptr; // Optional
    vertex_input_info.vertexBindingDescriptionCount = 1;
    vertex_input_info.vertexAttributeDescriptionCount = 3;
    vertex_input_info.pVertexBindingDescriptions = &binding_description;
    vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions;

    VkPipelineInputAssemblyStateCreateInfo input_assembly {};
    input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) m_swap_chain_extent.width;
    viewport.height = (float) m_swap_chain_extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor {};
    scissor.offset = {0, 0};
    scissor.extent = m_swap_chain_extent;

    VkPipelineViewportStateCreateInfo viewport_state {};
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = m_msaa_samples;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState color_blend_attachment {};
    color_blend_attachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT |
            VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable = VK_FALSE;
    color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
    color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineDepthStencilStateCreateInfo depth_stencil{};
    depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil.depthTestEnable = VK_TRUE;
    depth_stencil.depthWriteEnable = VK_TRUE;
    depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depth_stencil.depthBoundsTestEnable = VK_FALSE;
    depth_stencil.minDepthBounds = 0.0f; // Optional
    depth_stencil.maxDepthBounds = 1.0f; // Optional
    depth_stencil.stencilTestEnable = VK_FALSE;
    depth_stencil.front = {}; // Optional
    depth_stencil.back = {}; // Optional

    VkPipelineColorBlendStateCreateInfo color_blending {};
    color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.logicOpEnable = VK_FALSE;
    color_blending.logicOp = VK_LOGIC_OP_COPY; // Optional
    color_blending.attachmentCount = 1;
    color_blending.pAttachments = &color_blend_attachment;
    color_blending.blendConstants[0] = 0.0f; // Optional
    color_blending.blendConstants[1] = 0.0f; // Optional
    color_blending.blendConstants[2] = 0.0f; // Optional
    color_blending.blendConstants[3] = 0.0f; // Optional

    VkDynamicState dynamic_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamic_state {};
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.dynamicStateCount = 2;
    dynamic_state.pDynamicStates = dynamic_states;

    VkPipelineLayoutCreateInfo pipeline_layout_info {};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 1; // Optional
    pipeline_layout_info.pSetLayouts = &m_descriptor_set_layout; // Optional
    pipeline_layout_info.pushConstantRangeCount = 0; // Optional
    pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(m_surface_context->get_device()->get_handle(), &pipeline_layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout");
    }

    VkGraphicsPipelineCreateInfo pipeline_info {};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stages;

    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pDepthStencilState = &depth_stencil;
    pipeline_info.pColorBlendState = &color_blending;
    pipeline_info.pDynamicState = nullptr; // Optional

    pipeline_info.layout = m_pipeline_layout;
    pipeline_info.renderPass = m_render_pass;
    pipeline_info.subpass = 0;

    pipeline_info.basePipelineHandle = nullptr; // Optional
    pipeline_info.basePipelineIndex = -1; // Optional

    if (vkCreateGraphicsPipelines(m_surface_context->get_device()->get_handle(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_graphics_pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline");
    }

    vkDestroyShaderModule(m_surface_context->get_device()->get_handle(), vert_shader_module, nullptr);
    vkDestroyShaderModule(m_surface_context->get_device()->get_handle(), frag_shader_module, nullptr);
}

VkShaderModule HelloTriangleApplication::create_shader_module(const std::vector<char> &code) {
    VkShaderModuleCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shader_module {};
    if (vkCreateShaderModule(m_surface_context->get_device()->get_handle(), &createInfo, nullptr, &shader_module) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module");
    }

    return shader_module;
}

void HelloTriangleApplication::create_render_pass() {
    VkAttachmentDescription color_attachment {};
    color_attachment.format = m_swap_chain_image_format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;

    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    color_attachment.samples = m_msaa_samples;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference color_attachment_ref {};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depth_attachment {};
    depth_attachment.format = find_depth_format();
    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depth_attachment.samples = m_msaa_samples;

    VkAttachmentReference depth_attachment_ref {};
    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription color_attachment_resolve{};
    color_attachment_resolve.format = m_swap_chain_image_format;
    color_attachment_resolve.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment_resolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment_resolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment_resolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment_resolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment_resolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment_resolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_resolve_ref{};
    color_attachment_resolve_ref.attachment = 2;
    color_attachment_resolve_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;
    subpass.pResolveAttachments = &color_attachment_resolve_ref;

    VkSubpassDependency dependency {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcAccessMask = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkAttachmentDescription attachments[3] = { color_attachment, depth_attachment, color_attachment_resolve };

    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 3;
    render_pass_info.pAttachments = attachments;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.dependencyCount = 1;
    render_pass_info.pDependencies = &dependency;

    if (vkCreateRenderPass(m_surface_context->get_device()->get_handle(), &render_pass_info, nullptr, &m_render_pass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass");
    }
}

void HelloTriangleApplication::create_framebuffers() {
    m_swap_chain_framebuffers.resize(m_swap_chain_image_views.size());

    for (size_t i = 0; i < m_swap_chain_image_views.size(); i++) {
        VkImageView attachments[] = {
            m_color_image_view->get_handle(),
            m_depth_image_view->get_handle(),
            m_swap_chain_image_views[i]->get_handle()
        };

        VkFramebufferCreateInfo framebuffer_info {};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = m_render_pass;
        framebuffer_info.attachmentCount = 3,
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = m_swap_chain_extent.width;
        framebuffer_info.height = m_swap_chain_extent.height;
        framebuffer_info.layers = 1;

        if (vkCreateFramebuffer(m_surface_context->get_device()->get_handle(), &framebuffer_info, nullptr, &m_swap_chain_framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer");
        }
    }
}

void HelloTriangleApplication::create_command_pool() {
    m_surface_context->create_command_pool();
}

void HelloTriangleApplication::create_command_buffers() {
    m_command_buffers.resize(m_swap_chain_framebuffers.size());

    auto command_pool = m_surface_context->get_command_pool();

    for (int i = 0; i < m_command_buffers.size(); i++) {
        m_command_buffers[i] = std::make_unique<VK::CommandBuffer>(command_pool->create_command_buffer());
        auto command_buffer = m_command_buffers[i].get();

        command_buffer->get_wait_flags().push_back(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        command_buffer->begin(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

        VkRenderPassBeginInfo render_pass_info {};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = m_render_pass;
        render_pass_info.framebuffer = m_swap_chain_framebuffers[i];

        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.renderArea.extent = m_swap_chain_extent;

        VkClearValue clear_values[2] {};
        clear_values[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clear_values[1].depthStencil = {1.0f, 0};

        render_pass_info.clearValueCount = 2;
        render_pass_info.pClearValues = clear_values;

        vkCmdBeginRenderPass(command_buffer->get_handle(), &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(command_buffer->get_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);

        VkBuffer vertex_buffers[] = { m_vertex_buffer->get_handle() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(command_buffer->get_handle(), 0, 1, vertex_buffers, offsets);
        vkCmdBindIndexBuffer(command_buffer->get_handle(), m_index_buffer->get_handle(), 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(command_buffer->get_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline_layout, 0, 1, &m_descriptor_sets[i], 0, nullptr);
        vkCmdDrawIndexed(command_buffer->get_handle(), m_index_buffer_storage.size(), 1, 0, 0, 0);
        vkCmdEndRenderPass(command_buffer->get_handle());

        command_buffer->end();
    }
}

void HelloTriangleApplication::create_sync_objects() {

    m_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
    m_in_flight_images.resize(m_swap_chain_images.size(), nullptr);

    VkSemaphoreCreateInfo semaphore_info {};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if(vkCreateSemaphore(m_surface_context->get_device()->get_handle(), &semaphore_info, nullptr, &m_image_available_semaphores[i]) != VK_SUCCESS ||
           vkCreateSemaphore(m_surface_context->get_device()->get_handle(), &semaphore_info, nullptr, &m_render_finished_semaphores[i]) != VK_SUCCESS ||
           vkCreateFence(m_surface_context->get_device()->get_handle(), &fence_info, nullptr, &m_in_flight_fences[i]) != VK_SUCCESS) {

            throw std::runtime_error("failed to create synchronization objects for a frame");
        }
    }
}

void HelloTriangleApplication::draw_frame() {

    vkWaitForFences(m_surface_context->get_device()->get_handle(), 1, &m_in_flight_fences[m_current_frame], VK_TRUE, UINT64_MAX);

    uint32_t image_index = 0;
    auto result = vkAcquireNextImageKHR(m_surface_context->get_device()->get_handle(), m_swap_chain, UINT64_MAX, m_image_available_semaphores[m_current_frame], VK_NULL_HANDLE, &image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        m_framebuffer_resized = false;
        recreate_swap_chain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (m_in_flight_images[image_index] != nullptr) {
        vkWaitForFences(m_surface_context->get_device()->get_handle(), 1, &m_in_flight_images[image_index], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    m_in_flight_images[image_index] = m_in_flight_fences[m_current_frame];

    update_uniform_buffer(image_index);

    vkResetFences(m_surface_context->get_device()->get_handle(), 1, &m_in_flight_fences[m_current_frame]);

    auto command_buffer = m_command_buffers[image_index].get();

    command_buffer->get_wait_semaphores().assign({ m_image_available_semaphores[m_current_frame] });
    command_buffer->get_signal_semaphores().assign({ m_render_finished_semaphores[m_current_frame] });

    command_buffer->submit(m_surface_context->get_device_graphics_queue(),m_in_flight_fences[m_current_frame]);

    VkPresentInfoKHR present_info {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = m_command_buffers[image_index]->get_signal_semaphores().data();

    VkSwapchainKHR swap_chains[] = {m_swap_chain};
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swap_chains;
    present_info.pImageIndices = &image_index;
    present_info.pResults = nullptr; // Optional

    result = vkQueuePresentKHR(m_surface_context->get_device_graphics_queue(), &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebuffer_resized) {
        m_framebuffer_resized = false;
        recreate_swap_chain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    vkQueueWaitIdle(m_surface_context->get_device_present_queue());

    m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void HelloTriangleApplication::cleanup_swap_chain() {

    if(m_color_image_view) m_color_image_view->destroy();
    if(m_color_image) m_color_image->destroy();
    m_color_image_memory.free();

    if(m_depth_image_view) m_depth_image_view->destroy();
    if(m_depth_image) m_depth_image->destroy();
    m_depth_image_memory.free();

    m_uniform_buffers.clear();
    m_uniform_buffers_memory.clear();

    if(m_descriptor_pool) {
        vkDestroyDescriptorPool(m_surface_context->get_device()->get_handle(), m_descriptor_pool, nullptr);
        m_descriptor_pool = nullptr;
    }

    for (auto framebuffer : m_swap_chain_framebuffers) {
        if(framebuffer) {
            vkDestroyFramebuffer(m_surface_context->get_device()->get_handle(), framebuffer, nullptr);
        }
    }
    m_swap_chain_framebuffers.clear();

    if(m_graphics_pipeline) {
        vkDestroyPipeline(m_surface_context->get_device()->get_handle(), m_graphics_pipeline, nullptr);
        m_graphics_pipeline = nullptr;
    }

    if(m_pipeline_layout) {
        vkDestroyPipelineLayout(m_surface_context->get_device()->get_handle(), m_pipeline_layout, nullptr);
        m_pipeline_layout = nullptr;
    }

    if(m_render_pass) {
        vkDestroyRenderPass(m_surface_context->get_device()->get_handle(), m_render_pass, nullptr);
        m_render_pass = nullptr;
    }

    m_swap_chain_image_views.clear();

    if(m_swap_chain) {
        vkDestroySwapchainKHR(m_surface_context->get_device()->get_handle(), m_swap_chain, nullptr);
        m_swap_chain = nullptr;
    }
}

void HelloTriangleApplication::recreate_swap_chain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(m_window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(m_surface_context->get_device()->get_handle());

    cleanup_swap_chain();

    create_swap_chain();
    create_image_views();
    create_render_pass();
    create_graphics_pipeline();
    create_color_resources();
    create_depth_resources();
    create_framebuffers();
    create_uniform_buffers();
    create_descriptor_pool();
    create_descriptor_sets();
    create_command_buffers();
}

void HelloTriangleApplication::create_mesh() {
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

//            float length = sqrt(vertex[0] * vertex[0] + vertex[1] * vertex[1] + vertex[2] * vertex[2]);

//            length = 1 / sqrt(length);

//            vertex[0] = vertex[0] * length;
//            vertex[1] = vertex[1] * length;
//            vertex[2] = vertex[2] * length;

            for(auto num : vertex) {
                m_vertex_buffer_storage.push_back(num);
            }

            m_index_buffer_storage.push_back(m_index_buffer_storage.size());
        }
    }
}

void HelloTriangleApplication::create_index_buffer() {
    VkDeviceSize buffer_size = sizeof(m_index_buffer_storage[0]) * m_index_buffer_storage.size();

    VK::Memory staging_buffer_memory { m_surface_context->get_device() };
    VK::Buffer staging_buffer { &staging_buffer_memory };

    staging_buffer.set_size(buffer_size);
    staging_buffer.set_usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    staging_buffer.set_properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    staging_buffer.create();
    staging_buffer_memory.set_data(m_index_buffer_storage.data(), (size_t) buffer_size);

    m_index_buffer_memory.set_device(m_surface_context->get_device());
    m_index_buffer = std::make_unique<VK::Buffer>(&m_index_buffer_memory);
    m_index_buffer->set_size(buffer_size);
    m_index_buffer->set_usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    m_index_buffer->set_properties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    m_index_buffer->create();

    auto command_buffer = m_surface_context->get_command_pool()->create_command_buffer();
    command_buffer.begin(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    VK::CopyBufferCommand(&staging_buffer, m_index_buffer.get()).write(&command_buffer);

    command_buffer.end();
    command_buffer.submit_and_wait(m_surface_context->get_device_graphics_queue(), nullptr);
}

void HelloTriangleApplication::create_vertex_buffer() {

    VkDeviceSize buffer_size = sizeof(m_vertex_buffer_storage[0]) * m_vertex_buffer_storage.size();
    VK::Memory staging_buffer_memory {m_surface_context->get_device() };
    VK::Buffer staging_buffer {&staging_buffer_memory };

    staging_buffer.set_usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    staging_buffer.set_properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    staging_buffer.set_size(buffer_size);
    staging_buffer.create();
    staging_buffer_memory.set_data(m_vertex_buffer_storage.data(), (size_t) buffer_size);

    m_vertex_buffer_memory.set_device(m_surface_context->get_device());
    m_vertex_buffer = std::make_unique<VK::Buffer>(&m_vertex_buffer_memory);
    m_vertex_buffer->set_properties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    m_vertex_buffer->set_usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    m_vertex_buffer->set_size(buffer_size);
    m_vertex_buffer->create();

    auto command_buffer = m_surface_context->get_command_pool()->create_command_buffer();
    command_buffer.begin(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    VK::CopyBufferCommand(&staging_buffer, m_vertex_buffer.get()).write(&command_buffer);

    command_buffer.end();
    command_buffer.submit_and_wait(m_surface_context->get_device_graphics_queue(), nullptr);
}

void HelloTriangleApplication::create_descriptor_set_layout() {
    VkDescriptorSetLayoutBinding ubo_layout_binding {};
    ubo_layout_binding.binding = 0;
    ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    ubo_layout_binding.descriptorCount = 1;
    ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    ubo_layout_binding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding sampler_layout_binding {};
    sampler_layout_binding.binding = 1;
    sampler_layout_binding.descriptorCount = 1;
    sampler_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    sampler_layout_binding.pImmutableSamplers = nullptr;
    sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding bindings[] = { ubo_layout_binding, sampler_layout_binding };

    VkDescriptorSetLayoutCreateInfo layout_info {};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 2;
    layout_info.pBindings = bindings;

    if (vkCreateDescriptorSetLayout(m_surface_context->get_device()->get_handle(), &layout_info, nullptr, &m_descriptor_set_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout");
    }
}

void HelloTriangleApplication::create_uniform_buffers() {
    VkDeviceSize buffer_size = sizeof(UniformBufferObject);

    m_uniform_buffers.resize(m_swap_chain_images.size());
    m_uniform_buffers_memory.resize(m_swap_chain_images.size());

    for (size_t i = 0; i < m_swap_chain_images.size(); i++) {
        m_uniform_buffers_memory[i] = std::make_unique<VK::Memory>(m_surface_context->get_device());

        auto& buffer = m_uniform_buffers[i];
        buffer = std::make_unique<VK::Buffer>(m_uniform_buffers_memory[i].get());
        buffer->set_properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        buffer->set_usage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        buffer->set_size(buffer_size);
        buffer->create();
    }
}

void HelloTriangleApplication::update_uniform_buffer(uint32_t image_index) {

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
    ubo.proj = glm::perspective(glm::radians(45.0f), (float) m_swap_chain_extent.width / (float) m_swap_chain_extent.height, 0.01f, 100.0f);

    ubo.proj[1][1] *= -1;

    m_uniform_buffers_memory[image_index]->set_data(&ubo, sizeof(ubo));
}

void HelloTriangleApplication::create_descriptor_pool() {
    VkDescriptorPoolSize pool_size[2] {};
    pool_size[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_size[0].descriptorCount = static_cast<uint32_t>(m_swap_chain_images.size());

    pool_size[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    pool_size[1].descriptorCount = static_cast<uint32_t>(m_swap_chain_images.size());

    VkDescriptorPoolCreateInfo pool_info {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.poolSizeCount = 2;
    pool_info.pPoolSizes = pool_size;
    pool_info.maxSets = static_cast<uint32_t>(m_swap_chain_images.size());

    if (vkCreateDescriptorPool(m_surface_context->get_device()->get_handle(), &pool_info, nullptr, &m_descriptor_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool");
    }
}

void HelloTriangleApplication::create_descriptor_sets() {
    std::vector<VkDescriptorSetLayout> layouts(m_swap_chain_images.size(), m_descriptor_set_layout);
    VkDescriptorSetAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = m_descriptor_pool;
    alloc_info.descriptorSetCount = static_cast<uint32_t>(m_swap_chain_images.size());
    alloc_info.pSetLayouts = layouts.data();

    m_descriptor_sets.resize(m_swap_chain_images.size());
    if (vkAllocateDescriptorSets(m_surface_context->get_device()->get_handle(), &alloc_info, m_descriptor_sets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets");
    }

    for (size_t i = 0; i < m_swap_chain_images.size(); i++) {
        VkDescriptorBufferInfo buffer_info {};
        buffer_info.buffer = m_uniform_buffers[i]->get_handle();
        buffer_info.offset = 0;
        buffer_info.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo image_info {};
        image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        image_info.imageView = m_texture_image_view->get_handle();
        image_info.sampler = m_texture_sampler;

        VkWriteDescriptorSet descriptor_write[2] {};
        descriptor_write[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write[0].dstSet = m_descriptor_sets[i];
        descriptor_write[0].dstBinding = 0;
        descriptor_write[0].dstArrayElement = 0;
        descriptor_write[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_write[0].descriptorCount = 1;
        descriptor_write[0].pBufferInfo = &buffer_info;
        descriptor_write[0].pImageInfo = nullptr; // Optional
        descriptor_write[0].pTexelBufferView = nullptr; // Optional

        descriptor_write[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write[1].dstSet = m_descriptor_sets[i];
        descriptor_write[1].dstBinding = 1;
        descriptor_write[1].dstArrayElement = 0;
        descriptor_write[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptor_write[1].descriptorCount = 1;
        descriptor_write[1].pImageInfo = &image_info;

        vkUpdateDescriptorSets(m_surface_context->get_device()->get_handle(), 2, descriptor_write, 0, nullptr);
    }

}

void HelloTriangleApplication::create_texture_image() {
    const char* path = "resources/models/viking_room.png";
    FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(path, 0), path);
    FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    auto* image = (unsigned char*) FreeImage_GetBits(converted);
    int image_width = (int)FreeImage_GetWidth(converted);
    int image_height = (int)FreeImage_GetHeight(converted);
    int image_size = image_width * image_height * 4;

    m_mip_levels = (int) floor(log2(std::max(image_width, image_height))) + 1;

    for (int pix = 0; pix < image_size; pix += 4){
        std::swap(image[pix + 0], image[pix + 2]);
    }

    VK::Memory staging_buffer_memory { m_surface_context->get_device() };
    VK::Buffer staging_buffer { &staging_buffer_memory };

    staging_buffer.set_size(image_size);
    staging_buffer.set_usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    staging_buffer.set_properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    staging_buffer.create();
    staging_buffer_memory.set_data(image, image_size);

    FreeImage_Unload(converted);

    m_texture_image_memory.set_device(m_surface_context->get_device());

    m_texture_image = std::make_unique<VK::Image2D>(&m_texture_image_memory);
    m_texture_image->set_size(image_width, image_height);
    m_texture_image->set_mip_levels(m_mip_levels);
    m_texture_image->set_usage(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    m_texture_image->set_format(VK_FORMAT_R8G8B8A8_SRGB);
    m_texture_image->create();

    auto command_buffer = m_surface_context->get_command_pool()->create_command_buffer();
    command_buffer.begin(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    m_texture_image->perform_layout_transition(&command_buffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    auto copy_command = VK::CopyBufferToImageCommand(&staging_buffer, m_texture_image.get());
    copy_command.set_destination_image_layout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copy_command.write(&command_buffer);

//    m_texture_image->perform_layout_transition(&command_buffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    generate_mipmaps(&command_buffer, m_texture_image.get());

    command_buffer.end();
    command_buffer.submit_and_wait(m_surface_context->get_device_graphics_queue(), nullptr);

    m_texture_image_view = std::make_unique<VK::ImageView>(m_texture_image.get());
    m_texture_image_view->get_subresource_range().aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_texture_image_view->create();
}

void HelloTriangleApplication::generate_mipmaps(VK::CommandBuffer* command_buffer, VK::Image2D* image) {

    VkFormatProperties format_properties {};
    m_physical_device->get_format_properties(&format_properties, image->get_format());

    if (!(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        throw std::runtime_error("texture image format does not support linear blitting");
    }

    auto image_size = image->get_size();

    auto mip_width = (int32_t) image_size.width;
    auto mip_height = (int32_t) image_size.height;

    VkImageMemoryBarrier barrier {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image->get_handle();
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    for (uint32_t i = 1; i < m_mip_levels; i++) {

        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(command_buffer->get_handle(),
                             VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                             0, nullptr,
                             0, nullptr,
                             1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = { mip_width, mip_height, 1 };
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = { mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1 };
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        vkCmdBlitImage(command_buffer->get_handle(),
                       image->get_handle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                       image->get_handle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                       1, &blit,
                       VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(command_buffer->get_handle(),
                             VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                             0, nullptr,
                             0, nullptr,
                             1, &barrier);

        if (mip_width > 1) mip_width /= 2;
        if (mip_height > 1) mip_height /= 2;
    }

    barrier.subresourceRange.baseMipLevel = image->get_mip_levels() - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(command_buffer->get_handle(),
                         VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                         0, nullptr,
                         0, nullptr,
                         1, &barrier);
}

void HelloTriangleApplication::create_texture_sampler() {
    VkSamplerCreateInfo sampler_info {};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;

//    sampler_info.magFilter = VK_FILTER_NEAREST;
//    sampler_info.minFilter = VK_FILTER_NEAREST;

    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    sampler_info.anisotropyEnable = VK_TRUE;

    sampler_info.maxAnisotropy = m_physical_device->get_physical_properties()->limits.maxSamplerAnisotropy;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.unnormalizedCoordinates = VK_FALSE;
    sampler_info.compareEnable = VK_FALSE;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_info.mipLodBias = 0.0f;
//    sampler_info.maxLod = (float) m_mip_levels;
//    sampler_info.minLod = (float) 0.0f;

    sampler_info.maxLod = 0;
    sampler_info.minLod = 0;

    if (vkCreateSampler(m_surface_context->get_device()->get_handle(), &sampler_info, nullptr, &m_texture_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler");
    }
}

VkFormat HelloTriangleApplication::find_depth_format() {
    return m_physical_device->find_supported_format(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

bool HelloTriangleApplication::has_stencil_component(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void HelloTriangleApplication::create_depth_resources() {
    VkFormat depth_format = find_depth_format();

    m_depth_image_memory.set_device(m_surface_context->get_device());

    m_depth_image = std::make_unique<VK::Image2D>(&m_depth_image_memory);
    m_depth_image->set_samples(m_msaa_samples);
    m_depth_image->set_size(m_swap_chain_extent.width, m_swap_chain_extent.height);
    m_depth_image->set_usage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    m_depth_image->set_format(depth_format);
    m_depth_image->create();

    m_depth_image_view = std::make_unique<VK::ImageView>(m_depth_image.get());
    m_depth_image_view->get_subresource_range().aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    m_depth_image_view->create();

    auto command_buffer = m_surface_context->get_command_pool()->create_command_buffer();
    command_buffer.begin(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    m_depth_image->perform_layout_transition(&command_buffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    command_buffer.end();
    command_buffer.submit_and_wait(m_surface_context->get_device_graphics_queue(), nullptr);
}

void HelloTriangleApplication::create_color_resources() {
    VkFormat color_format = m_swap_chain_image_format;

    m_color_image_memory.set_device(m_surface_context->get_device());
    m_color_image = std::make_unique<VK::Image2D>(&m_color_image_memory);
    m_color_image->set_size(m_swap_chain_extent.width, m_swap_chain_extent.height);
    m_color_image->set_samples(m_msaa_samples);
    m_color_image->set_usage(VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    m_color_image->set_format(color_format);
    m_color_image->create();

    m_color_image_view = std::make_unique<VK::ImageView>(m_color_image.get());
    m_color_image_view->get_subresource_range().aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_color_image_view->create();
}