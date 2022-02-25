#pragma once

#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include "vk-device.hpp"
#include "vk-sampler.hpp"

namespace VK {

class SamplerFactory {
    VkSamplerCreateInfo m_description {};
public:
    SamplerFactory() {
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

    void set_mag_filter(VkFilter mag_filter) { m_description.magFilter = mag_filter; }
    void set_min_filter(VkFilter min_filter) { m_description.minFilter = min_filter; }
    void set_address_mode_u(VkSamplerAddressMode address_mode_u) { m_description.addressModeU = address_mode_u; }
    void set_address_mode_v(VkSamplerAddressMode address_mode_v) { m_description.addressModeV = address_mode_v; }
    void set_address_mode_w(VkSamplerAddressMode address_mode_w) { m_description.addressModeW = address_mode_w; }

    void set_address_modes_uv(VkSamplerAddressMode address_mode) {
        set_address_modes_uv(address_mode, address_mode);
    }

    void set_address_modes_uv(VkSamplerAddressMode address_mode_u, VkSamplerAddressMode address_mode_v) {
        m_description.addressModeU = address_mode_u;
        m_description.addressModeV = address_mode_v;
    }

    void set_address_modes_uvw(VkSamplerAddressMode address_mode) {
        set_address_modes_uvw(address_mode, address_mode, address_mode);
    }

    void set_address_modes_uvw(VkSamplerAddressMode address_mode_u, VkSamplerAddressMode address_mode_v, VkSamplerAddressMode address_mode_w) {
        m_description.addressModeU = address_mode_u;
        m_description.addressModeV = address_mode_v;
        m_description.addressModeV = address_mode_w;
    }

    void set_anisotropy_enable(VkBool32 anisotropy_enable) { m_description.anisotropyEnable = anisotropy_enable; }
    void set_max_anisotropy(float max_anisotropy) { m_description.maxAnisotropy = max_anisotropy; }
    void set_border_color(VkBorderColor border_color) { m_description.borderColor = border_color; }
    void set_unnormalized_coordinates(VkBool32 unnormalized_coordinates) { m_description.unnormalizedCoordinates = unnormalized_coordinates; }
    void set_compare_enable(VkBool32 compare_enable) { m_description.compareEnable = compare_enable; }
    void set_compare_op(VkCompareOp compare_op) { m_description.compareOp = compare_op; }
    void set_mipmap_mode(VkSamplerMipmapMode mipmap_mode) { m_description.mipmapMode = mipmap_mode; }
    void set_mip_lod_bias(float mip_lod_bias) { m_description.mipLodBias = mip_lod_bias; }
    void set_max_lod(float max_lod) { m_description.maxLod = max_lod; }
    void set_min_lod(float min_lod) { m_description.minLod = min_lod; }

    Sampler create(VK::Device* device) {
        VkSampler result;
        if (vkCreateSampler(device->get_handle(), &m_description, nullptr, &result) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler");
        }
        return { device, result };
    }
};

}