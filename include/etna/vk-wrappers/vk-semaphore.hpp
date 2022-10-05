#pragma once

#include <iostream>
#include <etna/volk.hpp>
#include <etna/vk-wrappers/resources/vk-device-resource.hpp>

namespace VK {

class SemaphoreBase: public DeviceResourceBase<VkSemaphore> {
public:
    using DeviceResourceBase::DeviceResourceBase;
    SemaphoreBase& operator=(SemaphoreBase&& move_assign) = default;

    ~SemaphoreBase() override = default;
};

using UnownedSemaphore = UnownedDeviceResource<VkSemaphore, SemaphoreBase>;

class Semaphore: public DeviceResource<VkSemaphore, SemaphoreBase> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    Semaphore(Semaphore&& move) noexcept = default;
    Semaphore& operator=(Semaphore&& move_assign) = default;

    static Semaphore create(VK::Device* device, VkSemaphoreCreateFlags flags = 0);

    ~Semaphore() override { destroy(); }

    void destroy() final;
};

}