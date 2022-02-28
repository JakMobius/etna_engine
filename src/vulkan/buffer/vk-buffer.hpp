#pragma once

#include <exception>
#include "../resources/vk-device-resource.hpp"

namespace VK {

using UnownedBuffer = UnownedDeviceResource<VkBuffer>;

class Buffer: public DeviceResource<VkBuffer> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    Buffer(Buffer&& move) noexcept = default;
    Buffer& operator=(Buffer&& move_assign) = default;

    ~Buffer() override { destroy(); }

    void destroy() final;
};

}