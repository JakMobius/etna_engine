#pragma once

#include <vulkan/vulkan_core.h>
#include "../vk-image.hpp"
#include "../../vk-device-resource.hpp"

namespace VK {

using UnownedImageView = UnownedDeviceResource<VkImageView>;

class ImageView: public DeviceResource<VkImageView> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    ImageView(ImageView&& move) noexcept = default;
    ImageView& operator=(ImageView&& move_assign) = default;

    ~ImageView() override { destroy(); }

    void destroy() {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyImageView(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }
};

}