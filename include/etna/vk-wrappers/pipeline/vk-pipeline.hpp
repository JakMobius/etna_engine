#pragma once

#include <etna/volk.hpp>
#include <etna/vk-wrappers/device/vk-device.hpp>
#include <etna/vk-wrappers/resources/vk-device-resource.hpp>

namespace VK {

using UnownedPipeline = UnownedDeviceResource<VkPipeline>;

class Pipeline : public DeviceResource<VkPipeline> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    Pipeline(Pipeline &&move) noexcept = default;
    Pipeline &operator=(Pipeline &&move_assign) = default;

    ~Pipeline() override { destroy(); }

    void destroy() final;
};
}