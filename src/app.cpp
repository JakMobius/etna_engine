//
// Created by Артем on 09.02.2022.
//

#include "app.hpp"
#include "codes/vk-physical-device-type-code.hpp"
#include "vk-swap-chain-support-details.hpp"
#include "shader-reader.hpp"
#include <set>

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

    if (enable_validation_layers) {
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

    if(enable_validation_layers) {
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
    create_graphics_pipeline();
    create_framebuffers();
    create_command_pool();
    create_command_buffers();
    create_semaphores();
}

void HelloTriangleApplication::create_surface() {
    if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void HelloTriangleApplication::setup_validation_layers() {
    if (enable_validation_layers && !check_validation_layer_support()) {
        throw std::runtime_error("some requested validation layers are not available");
    }
}

void HelloTriangleApplication::setup_debug_messenger() {
    if(enable_debug_messenger && !m_debug_callback_handler.listen(m_instance)) {
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

    return indices.is_complete() &&
           check_device_extension_support(device) &&
           VkSwapChainSupportDetails(device, m_surface).is_complete();
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
        glfwPollEvents();
        draw_frame();
    }
    vkDeviceWaitIdle(m_device);
}

void HelloTriangleApplication::cleanup() {
    m_debug_callback_handler.stop_listening();

    if(m_render_finished_semaphore) {
        vkDestroySemaphore(m_device, m_render_finished_semaphore, nullptr);
        m_render_finished_semaphore = nullptr;
    }

    if(m_image_available_semaphore) {
        vkDestroySemaphore(m_device, m_image_available_semaphore, nullptr);
        m_image_available_semaphore = nullptr;
    }

    if(m_command_pool) {
        vkDestroyCommandPool(m_device, m_command_pool, nullptr);
        m_command_pool = nullptr;
    }

    for (auto framebuffer : m_swap_chain_framebuffers) {
        vkDestroyFramebuffer(m_device, framebuffer, nullptr);
    }
    m_swap_chain_framebuffers.clear();

    if(m_graphics_pipeline) {
        vkDestroyPipeline(m_device, m_graphics_pipeline, nullptr);
    }

    if(m_render_pass) {
        vkDestroyRenderPass(m_device, m_render_pass, nullptr);
    }

    if(m_pipeline_layout) {
        vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);
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

void HelloTriangleApplication::init_window() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(m_window_width, m_window_height, "Vulkan", nullptr, nullptr);
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
    VkDeviceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    create_info.pQueueCreateInfos = queues_to_create.data();
    create_info.queueCreateInfoCount = queues_to_create.size();
    create_info.pEnabledFeatures = &device_features;

    create_info.enabledExtensionCount = m_device_extensions.size();
    create_info.ppEnabledExtensionNames = m_device_extensions.data();

    if(enable_validation_layers) {
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
        VkImageViewCreateInfo create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = m_swap_chain_images[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = m_swap_chain_image_format;
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(m_device, &create_info, nullptr, &m_swap_chain_image_views[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views");
        }
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

    VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

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
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
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
    pipeline_layout_info.setLayoutCount = 0; // Optional
    pipeline_layout_info.pSetLayouts = nullptr; // Optional
    pipeline_layout_info.pushConstantRangeCount = 0; // Optional
    pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(m_device, &pipeline_layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout");
    }

    VkGraphicsPipelineCreateInfo pipeline_info {};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stages;

    pipeline_info.pVertexInputState = &vertexInputInfo;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pDepthStencilState = nullptr; // Optional
    pipeline_info.pColorBlendState = &color_blending;
    pipeline_info.pDynamicState = nullptr; // Optional

    pipeline_info.layout = m_pipeline_layout;
    pipeline_info.renderPass = m_render_pass;
    pipeline_info.subpass = 0;

    pipeline_info.basePipelineHandle = nullptr; // Optional
    pipeline_info.basePipelineIndex = -1; // Optional

    if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_graphics_pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
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

    VkSubpassDescription subpass {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkSubpassDependency dependency {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;

    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;

    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 1;
    render_pass_info.pAttachments = &color_attachment;
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
            m_swap_chain_image_views[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_render_pass;
        framebufferInfo.attachmentCount = 1;
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

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clearColor;

        vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);
        vkCmdDraw(command_buffer, 3, 1, 0, 0);
        vkCmdEndRenderPass(command_buffer);

        if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer");
        }
    }
}

void HelloTriangleApplication::create_semaphores() {
    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_image_available_semaphore) != VK_SUCCESS ||
        vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_render_finished_semaphore) != VK_SUCCESS) {

        throw std::runtime_error("failed to create semaphores");
    }
}

void HelloTriangleApplication::draw_frame() {
    uint32_t image_index = 0;
    vkAcquireNextImageKHR(m_device, m_swap_chain, UINT64_MAX, m_image_available_semaphore, VK_NULL_HANDLE, &image_index);

    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { m_image_available_semaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = waitSemaphores;
    submit_info.pWaitDstStageMask = waitStages;

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_command_buffers[image_index];

    VkSemaphore signal_semaphores[] = {m_render_finished_semaphore };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    if (vkQueueSubmit(m_device_graphics_queue, 1, &submit_info, nullptr) != VK_SUCCESS) {
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

    vkQueuePresentKHR(m_device_graphics_queue, &present_info);

    vkQueueWaitIdle(m_device_present_queue);
}

