#pragma once

#include <etna/volk.hpp>
#include <stdexcept>
#include <etna/vk-wrappers/device/vk-device.hpp>
#include "vk-sampler.hpp"

namespace VK {

class SamplerFactory {
    VkSamplerCreateInfo m_description {};
public:
    SamplerFactory();

    SamplerFactory& set_mag_filter(VkFilter mag_filter) {
        m_description.magFilter = mag_filter;
        return *this;
    }

    SamplerFactory& set_min_filter(VkFilter min_filter) {
        m_description.minFilter = min_filter;
        return *this;
    }

    SamplerFactory& set_address_mode_u(VkSamplerAddressMode address_mode_u) {
        m_description.addressModeU = address_mode_u;
        return *this;
    }

    SamplerFactory& set_address_mode_v(VkSamplerAddressMode address_mode_v) {
        m_description.addressModeV = address_mode_v;
        return *this;
    }

    SamplerFactory& set_address_mode_w(VkSamplerAddressMode address_mode_w) {
        m_description.addressModeW = address_mode_w;
        return *this;
    }

    SamplerFactory& set_address_modes_uv(VkSamplerAddressMode address_mode) {
        set_address_modes_uv(address_mode, address_mode);
        return *this;
    }

    SamplerFactory& set_address_modes_uv(VkSamplerAddressMode address_mode_u, VkSamplerAddressMode address_mode_v);

    SamplerFactory& set_address_modes_uvw(VkSamplerAddressMode address_mode) {
        set_address_modes_uvw(address_mode, address_mode, address_mode);
        return *this;
    }

    SamplerFactory& set_address_modes_uvw(VkSamplerAddressMode address_mode_u, VkSamplerAddressMode address_mode_v, VkSamplerAddressMode address_mode_w);


    SamplerFactory& set_anisotropy_enable(VkBool32 anisotropy_enable) {
        m_description.anisotropyEnable = anisotropy_enable;
        return *this;
    }

    SamplerFactory& set_max_anisotropy(float max_anisotropy) {
        m_description.maxAnisotropy = max_anisotropy;
        return *this;
    }

    SamplerFactory& set_border_color(VkBorderColor border_color) {
        m_description.borderColor = border_color;
        return *this;
    }

    SamplerFactory& set_unnormalized_coordinates(VkBool32 unnormalized_coordinates) {
        m_description.unnormalizedCoordinates = unnormalized_coordinates;
        return *this;
    }

    SamplerFactory& set_compare_enable(VkBool32 compare_enable) {
        m_description.compareEnable = compare_enable;
        return *this;
    }

    SamplerFactory& set_compare_op(VkCompareOp compare_op) {
        m_description.compareOp = compare_op;
        return *this;
    }

    SamplerFactory& set_mipmap_mode(VkSamplerMipmapMode mipmap_mode) {
        m_description.mipmapMode = mipmap_mode;
        return *this;
    }

    SamplerFactory& set_mip_lod_bias(float mip_lod_bias) {
        m_description.mipLodBias = mip_lod_bias;
        return *this;
    }

    SamplerFactory& set_max_lod(float max_lod) {
        m_description.maxLod = max_lod;
        return *this;
    }

    SamplerFactory& set_min_lod(float min_lod) {
        m_description.minLod = min_lod;
        return *this;
    }

    Sampler create(VK::Device* device) {
        VkSampler result;
        if (vkCreateSampler(device->get_handle(), &m_description, nullptr, &result) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler");
        }
        return { device, result };
    }
};

}