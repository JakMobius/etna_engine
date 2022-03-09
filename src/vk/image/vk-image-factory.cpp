
#include <etna/vk-wrappers/image/vk-image-factory.hpp>

VK::ImageFactory::ImageFactory() {
    m_description.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    m_description.extent = {0, 0, 0};
    m_description.flags = 0;
    m_description.format = VK_FORMAT_UNDEFINED;
    m_description.mipLevels = 1;
    m_description.arrayLayers = 1;
    m_description.samples = VK_SAMPLE_COUNT_1_BIT;
    m_description.tiling = VK_IMAGE_TILING_OPTIMAL;
    m_description.usage = 0;
    m_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    m_description.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    m_description.imageType = VK_IMAGE_TYPE_2D;
    m_memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
}

VkImage VK::ImageFactory::create_raw_image(VK::Device* device) const {
    VkImage handle = nullptr;

    if (vkCreateImage(device->get_handle(), &m_description, nullptr, &handle) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image");
    }

    return handle;
}

VK::MemoryImage VK::ImageFactory::create(VK::Device* device) const {
    Image image {device, create_raw_image(device) };
    MemoryImage memory_image {std::move(image) };
    memory_image.create(m_memory_properties);
    return memory_image;
}