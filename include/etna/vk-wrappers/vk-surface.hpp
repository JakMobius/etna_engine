#pragma once

#include <etna/vk-wrappers/resources/vk-instance-resource.hpp>

namespace VK {
class SurfaceBase: public InstanceResourceBase<VkSurfaceKHR> {
public:
    using InstanceResourceBase::InstanceResourceBase;
    SurfaceBase& operator=(SurfaceBase&& move_assign) = default;
};

using UnownedSurface = UnownedInstanceResource<VkSurfaceKHR, SurfaceBase>;

class Surface: public InstanceResource<VkSurfaceKHR, SurfaceBase> {
public:
    using InstanceResource::InstanceResource;
    using InstanceResource::operator=;

    Surface(Surface&& move) noexcept = default;
    Surface& operator=(Surface&& move_assign) = default;

    ~Surface() override { destroy(); }

    void destroy() final;
};
}