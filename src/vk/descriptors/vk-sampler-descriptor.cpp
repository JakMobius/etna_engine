
#include <etna/vk-wrappers/descriptors/vk-sampler-descriptor.hpp>

VK::SamplerDescriptor::SamplerDescriptor(const VK::Sampler &sampler, const VK::ImageView &image_view) {
    m_description.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    m_description.descriptorCount = 1;
    m_description.pImageInfo = &m_image_info;

    m_image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    m_image_info.imageView = image_view.get_handle();
    m_image_info.sampler = sampler.get_handle();
}
