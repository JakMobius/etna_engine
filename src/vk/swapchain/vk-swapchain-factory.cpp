
#include <etna/vk-wrappers/swapchain/vk-swapchain-factory.hpp>

VK::SwapchainFactory::SwapchainFactory() {
    m_description.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    m_description.surface = VK_NULL_HANDLE;
    m_description.minImageCount = 0;
    m_description.imageFormat = VK_FORMAT_UNDEFINED;
    m_description.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    m_description.imageExtent = {0, 0};
    m_description.imageArrayLayers = 1;
    m_description.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    m_description.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    m_description.queueFamilyIndexCount = 0;
    m_description.pQueueFamilyIndices = nullptr;
    m_description.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    m_description.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    m_description.clipped = VK_TRUE;
    m_description.oldSwapchain = VK_NULL_HANDLE;
}

VK::Swapchain VK::SwapchainFactory::create(VK::Device* device) {

    m_description.queueFamilyIndexCount = m_queue_family_indices.size();
    m_description.pQueueFamilyIndices = m_queue_family_indices.data();

    VkSwapchainKHR handle = nullptr;

    if (vkCreateSwapchainKHR(device->get_handle(), &m_description, nullptr, &handle) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain");
    }

    return { device, handle };
}
