#pragma once

#include <etna/volk.hpp>
#include <etna/vk-wrappers/image/vk-image.hpp>
#include <etna/vk-wrappers/resources/vk-device-resource.hpp>

namespace VK {

using UnownedImageView = UnownedDeviceResource<VkImageView>;

class ImageView: public DeviceResource<VkImageView> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    ImageView(ImageView&& move) noexcept = default;
    ImageView& operator=(ImageView&& move_assign) = default;

    ~ImageView() override { destroy(); }

    void destroy() final;
};

}