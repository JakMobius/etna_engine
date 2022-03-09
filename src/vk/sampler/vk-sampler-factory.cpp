
#include <etna/vk-wrappers/sampler/vk-sampler-factory.hpp>

VK::SamplerFactory::SamplerFactory() {
    m_description.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    m_description.magFilter = VK_FILTER_NEAREST;
    m_description.minFilter = VK_FILTER_NEAREST;
    m_description.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    m_description.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    m_description.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    m_description.anisotropyEnable = VK_FALSE;
    m_description.maxAnisotropy = 1.0;
    m_description.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    m_description.unnormalizedCoordinates = VK_FALSE;
    m_description.compareEnable = VK_FALSE;
    m_description.compareOp = VK_COMPARE_OP_ALWAYS;
    m_description.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    m_description.mipLodBias = 0.0f;
    m_description.maxLod = 0;
    m_description.minLod = 0;
}

VK::SamplerFactory& VK::SamplerFactory::set_address_modes_uvw(VkSamplerAddressMode address_mode_u, VkSamplerAddressMode address_mode_v,
                                               VkSamplerAddressMode address_mode_w) {
    m_description.addressModeU = address_mode_u;
    m_description.addressModeV = address_mode_v;
    m_description.addressModeV = address_mode_w;
    return *this;
}

VK::SamplerFactory& VK::SamplerFactory::set_address_modes_uv(VkSamplerAddressMode address_mode_u, VkSamplerAddressMode address_mode_v) {
    m_description.addressModeU = address_mode_u;
    m_description.addressModeV = address_mode_v;
    return *this;
}
