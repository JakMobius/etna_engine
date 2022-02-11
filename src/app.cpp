//
// Created by Артем on 09.02.2022.
//

#include "app.hpp"
#include "codes/vk-physical-device-type-code.hpp"
#include "vk-swap-chain-support-details.hpp"
#include "shader-reader.hpp"
#include <FreeImage.h>
#include <set>
#include <random>
#include <chrono>
#include <thread>

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
        create_info.enabledLayerCount = static_cast<uint32_t>(required_validation_layers.size());
        create_info.ppEnabledLayerNames = required_validation_layers.data();
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
        std::cout << "\t" << ext.extensionName << " ver. " << VkVersionCode(ext.specVersion) << "\n";
    }
}

std::vector<const char*> HelloTriangleApplication::get_required_extensions() {
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
    pick_gpu();
    get_families();
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
    create_depth_resources();
    create_framebuffers();
    create_texture_image();
    create_texture_image_view();
    create_texture_sampler();
    create_uniform_buffers();
    create_descriptor_pool();
    create_descriptor_sets();
    create_command_buffers();
    create_sync_objects();
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

void HelloTriangleApplication::pick_gpu() {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);
    if(device_count == 0) {
        throw std::runtime_error("could not find any Vulkan-compatible GPU");
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

    VkPhysicalDevice best_gpu = select_best_gpu(devices);

    std::cout << "Devices:\n";
    for(int i = 0; i < device_count; i++) {
        VkPhysicalDeviceProperties properties {};
        VkPhysicalDeviceFeatures features {};

        vkGetPhysicalDeviceProperties(devices[i], &properties);
        vkGetPhysicalDeviceFeatures(devices[i], &features);

        std::cout << "\t[" << i << "]: " << properties.deviceName;
        if(devices[i] == best_gpu) std::cout << " ( CHOSEN )\n";
        else std::cout << "\n";

        std::cout << "\t\tapiVersion: " << VkVersionCode(properties.apiVersion) << "\n";
        std::cout << "\t\tdriverVersion: " << VkVersionCode(properties.driverVersion) << "\n";
        std::cout << "\t\tvendorID: " << properties.vendorID << "\n";
        std::cout << "\t\tdeviceID: " << properties.deviceID << "\n";
        std::cout << "\t\tdeviceType: " << VkPhysicalDeviceTypeCode(properties.deviceType) << "\n";
    }

    if (!best_gpu) {
        throw std::runtime_error("could not find any suitable GPU");
    }

    m_gpu = best_gpu;
}

VkPhysicalDevice HelloTriangleApplication::select_best_gpu(const std::vector<VkPhysicalDevice>& devices) {
    VkPhysicalDevice discrete_gpu = nullptr;
    VkPhysicalDevice any_suitable_gpu = nullptr;

    for(auto device : devices) {
        VkPhysicalDeviceProperties properties {};
        vkGetPhysicalDeviceProperties(device, &properties);

        if(!is_device_suitable(device, &properties)) continue;

        if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            discrete_gpu = device;
            break;
        } else {
            any_suitable_gpu = device;
        }
    }

    if(discrete_gpu) return discrete_gpu;
    return any_suitable_gpu;
}

bool HelloTriangleApplication::is_device_suitable(VkPhysicalDevice device, VkPhysicalDeviceProperties* properties) {
    VkQueueFamilyIndices indices(device);
    indices.find_graphics_family();

    VkPhysicalDeviceFeatures supported_features {};
    vkGetPhysicalDeviceFeatures(device, &supported_features);

    return indices.is_complete() &&
           check_device_extension_support(device) &&
           VkSwapChainSupportDetails(device, m_surface).is_complete() &&
           supported_features.samplerAnisotropy;
}

bool HelloTriangleApplication::check_validation_layer_support() {
    bool result = true;

    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    std::cout << "Supported validation layers:\n";

    for(auto& layer : available_layers) {
        std::cout << "\t" << layer.layerName << " ver. " << VkVersionCode(layer.specVersion) << " impl. " << VkVersionCode(layer.implementationVersion) << ": " << layer.description << "\n";
    }

    for (const char* required_layer : required_validation_layers) {
        bool layerFound = false;

        for (const auto& available_layer : available_layers) {
            if (strcmp(required_layer, available_layer.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            result = false;
            std::cout << "Warning: Validation layer '" << required_layer << "' is required but not avabilable\n";
        }
    }

    return result;
}

void HelloTriangleApplication::main_loop() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwWaitEventsTimeout(1.0 / 60.0);

        draw_frame();
    }
    vkDeviceWaitIdle(m_device);
}

void HelloTriangleApplication::cleanup() {

    cleanup_swap_chain();

    if(m_depth_image_view) {
        vkDestroyImageView(m_device, m_depth_image_view, nullptr);
        m_depth_image_view = nullptr;
    }

    if(m_depth_image_memory) {
        vkFreeMemory(m_device, m_depth_image_memory, nullptr);
        m_depth_image_memory = nullptr;
    }

    if(m_depth_image) {
        vkDestroyImage(m_device, m_depth_image, nullptr);
        m_depth_image = nullptr;
    }

    if(m_texture_sampler) {
        vkDestroySampler(m_device, m_texture_sampler, nullptr);
        m_texture_sampler = nullptr;
    }

    if(m_texture_image_view) {
        vkDestroyImageView(m_device, m_texture_image_view, nullptr);
        m_texture_image_view = nullptr;
    }

    if(m_texture_image) {
        vkDestroyImage(m_device, m_texture_image, nullptr);
        m_texture_image = nullptr;
    }

    if(m_texture_image_memory) {
        vkFreeMemory(m_device, m_texture_image_memory, nullptr);
        m_texture_image_memory = nullptr;
    }

    if(m_descriptor_set_layout) {
        vkDestroyDescriptorSetLayout(m_device, m_descriptor_set_layout, nullptr);
        m_descriptor_set_layout = nullptr;
    }

    if(m_index_buffer_memory) {
        vkFreeMemory(m_device, m_index_buffer_memory, nullptr);
        m_index_buffer_memory = nullptr;
    }

    if(m_index_buffer) {
        vkDestroyBuffer(m_device, m_index_buffer, nullptr);
        m_index_buffer = nullptr;
    }

    if(m_vertex_buffer_memory) {
        vkFreeMemory(m_device, m_vertex_buffer_memory, nullptr);
        m_vertex_buffer_memory = nullptr;
    }

    if(m_vertex_buffer) {
        vkDestroyBuffer(m_device, m_vertex_buffer, nullptr);
        m_vertex_buffer = nullptr;
    }

    m_debug_callback_handler.stop_listening();

    for(auto& fence : m_in_flight_fences) {
        vkDestroyFence(m_device, fence, nullptr);
    }
    m_in_flight_fences.clear();

    for (auto& semaphore : m_image_available_semaphores) {
        vkDestroySemaphore(m_device, semaphore, nullptr);
    }
    m_image_available_semaphores.clear();

    for (auto& semaphore : m_render_finished_semaphores) {
        vkDestroySemaphore(m_device, semaphore, nullptr);
    }
    m_render_finished_semaphores.clear();

    if(m_command_pool) {
        vkDestroyCommandPool(m_device, m_command_pool, nullptr);
        m_command_pool = nullptr;
    }

    if(m_device) {
        vkDestroyDevice(m_device, nullptr);
        m_device = nullptr;
    }

    if(m_surface) {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        m_surface = nullptr;
    }

    vkDestroyInstance(m_instance, nullptr);

    m_gpu = nullptr;

    glfwDestroyWindow(m_window);

    glfwTerminate();
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

void HelloTriangleApplication::get_families() {
    m_family_indices = std::make_unique<VkQueueFamilyIndices>(m_gpu);
    m_family_indices->find_graphics_family();
}

void HelloTriangleApplication::create_logical_device() {

    m_family_indices->find_present_family(m_surface);

    std::vector<VkDeviceQueueCreateInfo> queues_to_create;
    std::set<int> unique_queue_families = {
        m_family_indices->m_graphics_family,
        m_family_indices->m_present_family
    };

    float queue_priority = 1.0f;

    for (int queue_family : unique_queue_families) {
        VkDeviceQueueCreateInfo queue_create_info {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queues_to_create.push_back(queue_create_info);
    }

    VkPhysicalDeviceFeatures device_features {};
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    create_info.pQueueCreateInfos = queues_to_create.data();
    create_info.queueCreateInfoCount = queues_to_create.size();
    create_info.pEnabledFeatures = &device_features;

    create_info.enabledExtensionCount = m_device_extensions.size();
    create_info.ppEnabledExtensionNames = m_device_extensions.data();

    if(m_enable_validation_layers) {
        create_info.enabledLayerCount = required_validation_layers.size();
        create_info.ppEnabledLayerNames = required_validation_layers.data();
    } else {
        create_info.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_gpu, &create_info, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

    vkGetDeviceQueue(m_device, m_family_indices->m_graphics_family, 0, &m_device_graphics_queue);
    vkGetDeviceQueue(m_device, m_family_indices->m_present_family, 0, &m_device_present_queue);
}

bool HelloTriangleApplication::check_device_extension_support(VkPhysicalDevice physical_device) {
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());

    std::set<std::string> required_extensions(m_device_extensions.begin(), m_device_extensions.end());

    for (const auto& extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

void HelloTriangleApplication::create_swap_chain() {

    VkSwapChainSupportDetails swap_chain_support(m_gpu, m_surface);

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
        (uint32_t) m_family_indices->m_graphics_family,
        (uint32_t) m_family_indices->m_present_family
    };

    if (m_family_indices->m_graphics_family != m_family_indices->m_present_family) {
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

    if (vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swap_chain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain");
    }

    vkGetSwapchainImagesKHR(m_device, m_swap_chain, &image_count, nullptr);
    m_swap_chain_images.resize(image_count);
    vkGetSwapchainImagesKHR(m_device, m_swap_chain, &image_count, m_swap_chain_images.data());

    m_swap_chain_image_format = surface_format.format;
    m_swap_chain_extent = extent;
}

void HelloTriangleApplication::create_image_views() {
    m_swap_chain_image_views.resize(m_swap_chain_images.size());

    for (size_t i = 0; i < m_swap_chain_image_views.size(); i++) {
        m_swap_chain_image_views[i] = create_image_view(m_swap_chain_images[i], m_swap_chain_image_format, VK_IMAGE_ASPECT_COLOR_BIT);
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
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
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

    if (vkCreatePipelineLayout(m_device, &pipeline_layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS) {
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

    if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_graphics_pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline");
    }

    vkDestroyShaderModule(m_device, vert_shader_module, nullptr);
    vkDestroyShaderModule(m_device, frag_shader_module, nullptr);
}

VkShaderModule HelloTriangleApplication::create_shader_module(const std::vector<char> &code) {
    VkShaderModuleCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shader_module {};
    if (vkCreateShaderModule(m_device, &createInfo, nullptr, &shader_module) != VK_SUCCESS) {
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
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

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

    VkAttachmentReference depth_attachment_ref {};
    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;

    VkSubpassDependency dependency {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcAccessMask = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkAttachmentDescription attachments[2] = { color_attachment, depth_attachment };

    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 2;
    render_pass_info.pAttachments = attachments;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.dependencyCount = 1;
    render_pass_info.pDependencies = &dependency;

    if (vkCreateRenderPass(m_device, &render_pass_info, nullptr, &m_render_pass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass");
    }
}

void HelloTriangleApplication::create_framebuffers() {
    m_swap_chain_framebuffers.resize(m_swap_chain_image_views.size());

    for (size_t i = 0; i < m_swap_chain_image_views.size(); i++) {
        VkImageView attachments[] = {
            m_swap_chain_image_views[i],
            m_depth_image_view
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_render_pass;
        framebufferInfo.attachmentCount = 2,
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_swap_chain_extent.width;
        framebufferInfo.height = m_swap_chain_extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_swap_chain_framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer");
        }
    }
}

void HelloTriangleApplication::create_command_pool() {

    VkCommandPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = m_family_indices->m_graphics_family;
    pool_info.flags = 0; // Optional

    if (vkCreateCommandPool(m_device, &pool_info, nullptr, &m_command_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool");
    }
}

void HelloTriangleApplication::create_command_buffers() {
    m_command_buffers.resize(m_swap_chain_framebuffers.size());

    VkCommandBufferAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = m_command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = (uint32_t) m_command_buffers.size();

    if (vkAllocateCommandBuffers(m_device, &alloc_info, m_command_buffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }

    for (int i = 0; i < m_command_buffers.size(); i++) {
        auto command_buffer = m_command_buffers[i];

        VkCommandBufferBeginInfo begin_info {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = 0; // Optional
        begin_info.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer");
        }

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

        vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);

        VkBuffer vertex_buffers[] = { m_vertex_buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
        vkCmdBindIndexBuffer(command_buffer, m_index_buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline_layout, 0, 1, &m_descriptor_sets[i], 0, nullptr);
        vkCmdDrawIndexed(command_buffer, m_index_buffer_storage.size(), 1, 0, 0, 0);
        vkCmdEndRenderPass(command_buffer);

        if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer");
        }
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
        if(vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_image_available_semaphores[i]) != VK_SUCCESS ||
           vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_render_finished_semaphores[i]) != VK_SUCCESS ||
           vkCreateFence(m_device, &fence_info, nullptr, &m_in_flight_fences[i]) != VK_SUCCESS) {

            throw std::runtime_error("failed to create synchronization objects for a frame");
        }
    }
}

void HelloTriangleApplication::draw_frame() {

    vkWaitForFences(m_device, 1, &m_in_flight_fences[m_current_frame], VK_TRUE, UINT64_MAX);

    uint32_t image_index = 0;
    auto result = vkAcquireNextImageKHR(m_device, m_swap_chain, UINT64_MAX, m_image_available_semaphores[m_current_frame], VK_NULL_HANDLE, &image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        m_framebuffer_resized = false;
        recreate_swap_chain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (m_in_flight_images[image_index] != nullptr) {
        vkWaitForFences(m_device, 1, &m_in_flight_images[image_index], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    m_in_flight_images[image_index] = m_in_flight_fences[m_current_frame];

    update_uniform_buffer(image_index);

    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { m_image_available_semaphores[m_current_frame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = waitSemaphores;
    submit_info.pWaitDstStageMask = waitStages;

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_command_buffers[image_index];

    VkSemaphore signal_semaphores[] = {m_render_finished_semaphores[m_current_frame] };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    vkResetFences(m_device, 1, &m_in_flight_fences[m_current_frame]);

    if (vkQueueSubmit(m_device_graphics_queue, 1, &submit_info, m_in_flight_fences[m_current_frame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    VkPresentInfoKHR present_info {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;

    VkSwapchainKHR swap_chains[] = {m_swap_chain};
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

void HelloTriangleApplication::cleanup_swap_chain() {

    for(auto uniform_buffer : m_uniform_buffers) {
        if(uniform_buffer) {
            vkDestroyBuffer(m_device, uniform_buffer, nullptr);
        }
    }
    for(auto uniform_buffer_memory : m_uniform_buffers_memory) {
        if(uniform_buffer_memory) {
            vkFreeMemory(m_device, uniform_buffer_memory, nullptr);
        }
    }
    m_uniform_buffers.clear();
    m_uniform_buffers_memory.clear();

    if(m_descriptor_pool) {
        vkDestroyDescriptorPool(m_device, m_descriptor_pool, nullptr);
        m_descriptor_pool = nullptr;
    }

    for (auto framebuffer : m_swap_chain_framebuffers) {
        if(framebuffer) {
            vkDestroyFramebuffer(m_device, framebuffer, nullptr);
        }
    }
    m_swap_chain_framebuffers.clear();

    vkFreeCommandBuffers(m_device, m_command_pool, m_command_buffers.size(), m_command_buffers.data());

    if(m_graphics_pipeline) {
        vkDestroyPipeline(m_device, m_graphics_pipeline, nullptr);
        m_graphics_pipeline = nullptr;
    }

    if(m_pipeline_layout) {
        vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);
        m_pipeline_layout = nullptr;
    }

    if(m_render_pass) {
        vkDestroyRenderPass(m_device, m_render_pass, nullptr);
        m_render_pass = nullptr;
    }

    for(auto image_view : m_swap_chain_image_views) {
        if(image_view) {
            vkDestroyImageView(m_device, image_view, nullptr);
        }
    }
    m_swap_chain_image_views.clear();

    if(m_swap_chain) {
        vkDestroySwapchainKHR(m_device, m_swap_chain, nullptr);
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

    vkDeviceWaitIdle(m_device);

    cleanup_swap_chain();

    create_swap_chain();
    create_image_views();
    create_render_pass();
    create_graphics_pipeline();
    create_framebuffers();
    create_uniform_buffers();
    create_descriptor_pool();
    create_descriptor_sets();
    create_command_buffers();
}

uint32_t HelloTriangleApplication::find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(m_gpu, &mem_properties);

    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
        if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void HelloTriangleApplication::create_mesh() {
//    std::mt19937 random { 0 };
//
//    int index = 0;
//
//    for(int i = 0; i < 10000; i++) {
//
//        float rx = random() / (float)std::mt19937::max() * 1 - 0.5;
//        float ry = random() / (float)std::mt19937::max() * 1 - 0.5;
//
//        float triangle[] = {
//                rx + 0.00f, ry - 0.05f, 0.0f, 1.0f, 0.0f,
//                rx + 0.05f, ry + 0.05f, 1.0f, 0.0f, 0.0f,
//                rx - 0.05f, ry + 0.05f, 0.0f, 0.0f, 1.0f
//        };
//
//        for(int j = 0; j < 15; j++) {
//            m_vertex_buffer_storage.push_back(triangle[j]);
//        }
//        m_index_buffer_storage.push_back(index);
//        m_index_buffer_storage.push_back(index + 1);
//        m_index_buffer_storage.push_back(index + 2);
//
//        index += 3;
//    }

    m_vertex_buffer_storage.assign({
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f,  -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

        0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f,  0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    });

    m_index_buffer_storage.assign({
          0, 1, 2, 2, 3, 0,
          4, 6, 4, 6, 4, 7,
          8, 9, 10, 10, 11, 8,
          12, 14, 13, 14, 12, 15,
          16, 18, 17, 18, 16, 19,
          20, 21, 22, 22, 23, 20,
    });
}

void HelloTriangleApplication::create_index_buffer() {
    VkBuffer m_staging_buffer {};
    VkDeviceMemory m_staging_buffer_memory {};

    VkDeviceSize buffer_size = sizeof(m_index_buffer_storage[0]) * m_index_buffer_storage.size();
    create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, m_staging_buffer, m_staging_buffer_memory);

    void* data = nullptr;
    vkMapMemory(m_device, m_staging_buffer_memory, 0, buffer_size, 0, &data);
    memcpy(data, m_index_buffer_storage.data(), (size_t) buffer_size);

    VkMappedMemoryRange flush_range = {
            VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
            nullptr,
            m_staging_buffer_memory,
            0,
            buffer_size
    };

    vkFlushMappedMemoryRanges(m_device, 1, &flush_range);
    vkUnmapMemory(m_device, m_staging_buffer_memory);

    create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_index_buffer, m_index_buffer_memory);

    copy_buffer(m_staging_buffer, m_index_buffer, buffer_size);

    vkDestroyBuffer(m_device, m_staging_buffer, nullptr);
    vkFreeMemory(m_device, m_staging_buffer_memory, nullptr);
}

void HelloTriangleApplication::create_vertex_buffer() {

    VkBuffer m_staging_buffer {};
    VkDeviceMemory m_staging_buffer_memory {};

    VkDeviceSize buffer_size = sizeof(m_vertex_buffer_storage[0]) * m_vertex_buffer_storage.size();
    create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, m_staging_buffer, m_staging_buffer_memory);

    void* data = nullptr;
    vkMapMemory(m_device, m_staging_buffer_memory, 0, buffer_size, 0, &data);
    memcpy(data, m_vertex_buffer_storage.data(), (size_t) buffer_size);

    VkMappedMemoryRange flush_range = {
        VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
        nullptr,
        m_staging_buffer_memory,
        0,
        buffer_size
    };

    vkFlushMappedMemoryRanges(m_device, 1, &flush_range);
    vkUnmapMemory(m_device, m_staging_buffer_memory);

    create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertex_buffer, m_vertex_buffer_memory);

    copy_buffer(m_staging_buffer, m_vertex_buffer, buffer_size);

    vkDestroyBuffer(m_device, m_staging_buffer, nullptr);
    vkFreeMemory(m_device, m_staging_buffer_memory, nullptr);
}

void HelloTriangleApplication::copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size) {
    VkCommandBuffer command_buffer = begin_single_time_commands();

    VkBufferCopy copy_region {};
    copy_region.srcOffset = 0;
    copy_region.dstOffset = 0;
    copy_region.size = size;

    vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);

    end_single_time_commands(command_buffer);
}

void HelloTriangleApplication::create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo buffer_info{};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_device, &buffer_info, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer");
    }

    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(m_device, buffer, &mem_requirements);

    VkMemoryAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex = find_memory_type(mem_requirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_device, &alloc_info, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory");
    }

    vkBindBufferMemory(m_device, buffer, bufferMemory, 0);
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

    if (vkCreateDescriptorSetLayout(m_device, &layout_info, nullptr, &m_descriptor_set_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout");
    }
}

void HelloTriangleApplication::create_uniform_buffers() {
    VkDeviceSize buffer_size = sizeof(UniformBufferObject);

    m_uniform_buffers.resize(m_swap_chain_images.size());
    m_uniform_buffers_memory.resize(m_swap_chain_images.size());

    for (size_t i = 0; i < m_swap_chain_images.size(); i++) {
        create_buffer(buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniform_buffers[i], m_uniform_buffers_memory[i]);
    }
}

void HelloTriangleApplication::update_uniform_buffer(uint32_t image_index) {

//    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    m_time += 1.0f / 60.0f;

    UniformBufferObject ubo {};
    ubo.model = glm::rotate(glm::mat4(1.0f), m_time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), m_swap_chain_extent.width / (float) m_swap_chain_extent.height, 0.1f, 10.0f);

    ubo.proj[1][1] *= -1;

    void* data = nullptr;
    vkMapMemory(m_device, m_uniform_buffers_memory[image_index], 0, sizeof(ubo), 0, &data);

    VkMappedMemoryRange flush_range = {
            VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
            nullptr,
            m_uniform_buffers_memory[image_index],
            0,
            sizeof(ubo)
    };

    memcpy(data, &ubo, sizeof(ubo));
    vkFlushMappedMemoryRanges(m_device, 1, &flush_range);
    vkUnmapMemory(m_device, m_uniform_buffers_memory[image_index]);
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

    if (vkCreateDescriptorPool(m_device, &pool_info, nullptr, &m_descriptor_pool) != VK_SUCCESS) {
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
    if (vkAllocateDescriptorSets(m_device, &alloc_info, m_descriptor_sets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets");
    }

    for (size_t i = 0; i < m_swap_chain_images.size(); i++) {
        VkDescriptorBufferInfo buffer_info {};
        buffer_info.buffer = m_uniform_buffers[i];
        buffer_info.offset = 0;
        buffer_info.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo image_info {};
        image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        image_info.imageView = m_texture_image_view;
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

        vkUpdateDescriptorSets(m_device, 2, descriptor_write, 0, nullptr);
    }

}

void HelloTriangleApplication::create_image(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& image_memory) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(m_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_device, image, &memRequirements);

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = memRequirements.size;
    alloc_info.memoryTypeIndex = find_memory_type(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_device, &alloc_info, nullptr, &image_memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(m_device, image, image_memory, 0);
}

void HelloTriangleApplication::create_texture_image() {
    const char* path = "resources/img/texture-2.png";
    FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(path, 0), path);
    FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    auto* image = (unsigned char*) FreeImage_GetBits(converted);
    int image_width = (int)FreeImage_GetWidth(converted);
    int image_height = (int)FreeImage_GetHeight(converted);
    int image_size = image_width * image_height * 4;

    for (int pix = 0; pix < image_size; pix += 4){
        std::swap(image[pix + 0], image[pix + 2]);
    }

    VkBuffer staging_buffer = nullptr;
    VkDeviceMemory staging_buffer_memory = nullptr;

    create_buffer(image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_memory);

    void* data = nullptr;
    vkMapMemory(m_device, staging_buffer_memory, 0, image_size, 0, &data);
    memcpy(data, image, image_size);
    vkUnmapMemory(m_device, staging_buffer_memory);
    FreeImage_Unload(converted);

    create_image(image_width, image_height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_texture_image, m_texture_image_memory);

    transition_image_layout(m_texture_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copy_buffer_to_image(staging_buffer, m_texture_image, image_width, image_height);
    transition_image_layout(m_texture_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(m_device, staging_buffer, nullptr);
    vkFreeMemory(m_device, staging_buffer_memory, nullptr);
}

void HelloTriangleApplication::transition_image_layout(VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout) {
    VkCommandBuffer command_buffer = begin_single_time_commands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;

    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags source_stage = 0;
    VkPipelineStageFlags destination_stage = 0;

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
            command_buffer,
            source_stage, destination_stage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
    );

    end_single_time_commands(command_buffer);
}

void HelloTriangleApplication::copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = begin_single_time_commands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    end_single_time_commands(commandBuffer);
}

VkImageView HelloTriangleApplication::create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspect_flags) {
    VkImageViewCreateInfo view_info {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.subresourceRange.aspectMask = aspect_flags;

    VkImageView image_view = nullptr;
    if (vkCreateImageView(m_device, &view_info, nullptr, &image_view) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return image_view;
}

void HelloTriangleApplication::create_texture_image_view() {
    m_texture_image_view = create_image_view(m_texture_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

void HelloTriangleApplication::create_texture_sampler() {
    VkSamplerCreateInfo sampler_info {};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
//    sampler_info.magFilter = VK_FILTER_LINEAR;
//    sampler_info.minFilter = VK_FILTER_LINEAR;

    sampler_info.magFilter = VK_FILTER_NEAREST;
    sampler_info.minFilter = VK_FILTER_NEAREST;

    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    sampler_info.anisotropyEnable = VK_TRUE;

    VkPhysicalDeviceProperties properties {};
    vkGetPhysicalDeviceProperties(m_gpu, &properties);
    sampler_info.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.unnormalizedCoordinates = VK_FALSE;
    sampler_info.compareEnable = VK_FALSE;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_info.mipLodBias = 0.0f;
    sampler_info.minLod = 0.0f;
    sampler_info.maxLod = 0.0f;

    if (vkCreateSampler(m_device, &sampler_info, nullptr, &m_texture_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler");
    }
}

VkFormat HelloTriangleApplication::find_supported_format(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props {};
        vkGetPhysicalDeviceFormatProperties(m_gpu, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format");
}

VkFormat HelloTriangleApplication::find_depth_format() {
    return find_supported_format(
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

bool HelloTriangleApplication::has_stencil_component(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void HelloTriangleApplication::create_depth_resources() {
    VkFormat depth_format = find_depth_format();

    create_image(m_swap_chain_extent.width, m_swap_chain_extent.height, depth_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depth_image, m_depth_image_memory);
    m_depth_image_view = create_image_view(m_depth_image, depth_format, VK_IMAGE_ASPECT_DEPTH_BIT);

    transition_image_layout(m_depth_image, depth_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

VkCommandBuffer HelloTriangleApplication::begin_single_time_commands() {
    VkCommandBufferAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = m_command_pool;
    alloc_info.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(m_device, &alloc_info, &command_buffer);

    VkCommandBufferBeginInfo begin_info {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &begin_info);

    return command_buffer;
}

void HelloTriangleApplication::end_single_time_commands(VkCommandBuffer command_buffer) {
    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    vkQueueSubmit(m_device_graphics_queue, 1, &submit_info, nullptr);
    vkQueueWaitIdle(m_device_graphics_queue);

    vkFreeCommandBuffers(m_device, m_command_pool, 1, &command_buffer);
}
