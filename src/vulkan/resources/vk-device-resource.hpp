#pragma once

#include <vulkan/vulkan_core.h>
#include "../vk-device.hpp"
#include "vk-resource.hpp"

namespace VK {

template<typename Handle>
class DeviceResourceBase : public ResourceBase<Handle> {
protected:
    Device* m_device;

    DeviceResourceBase(Device* device, Handle handle): ResourceBase<Handle>(handle), m_device(device) {}
public:

    DeviceResourceBase(const DeviceResourceBase& copy) = delete;
    DeviceResourceBase& operator=(const DeviceResourceBase& copy_assign) = delete;
    DeviceResourceBase& operator=(DeviceResourceBase&& move_assign) noexcept {
        this->m_handle = move_assign.m_handle;
        m_device = move_assign.m_device;
        move_assign.m_handle = nullptr;
        return *this;
    }

    virtual ~DeviceResourceBase() = default;

    Device* get_device() const { return m_device; };
};

template<typename Handle, typename Base = DeviceResourceBase<Handle>>
class UnownedDeviceResource : public Base {
public:
    UnownedDeviceResource(VK::Device* device, Handle handle): Base(device, handle) {}
    UnownedDeviceResource(): Base(nullptr, nullptr) {}
    UnownedDeviceResource(UnownedDeviceResource&& move) noexcept: Base(move.m_device, move.m_handle) { move.m_handle = nullptr; }
    UnownedDeviceResource(const UnownedDeviceResource& copy): Base(copy.m_device, copy.m_handle) {};
    UnownedDeviceResource& operator=(UnownedDeviceResource&& move_assign) noexcept = default;
    UnownedDeviceResource& operator=(const UnownedDeviceResource& copy_assign) {
        this->m_handle = copy_assign.m_handle;
        this->m_device = copy_assign.m_device;
        return *this;
    }
};

template<typename Handle, typename Base = DeviceResourceBase<Handle>>
class DeviceResource : public Base {

public:
    DeviceResource(Device* device, Handle handle): Base(device, handle) {}
    DeviceResource(): Base(nullptr, nullptr) {}
    DeviceResource(DeviceResource&& move) noexcept: Base(move.m_device, move.m_handle) { move.m_handle = nullptr; }
    DeviceResource& operator=(DeviceResource&& move_assign) noexcept = default;
    DeviceResource& operator=(const DeviceResource& copy_assign) = default;

    ~DeviceResource() override = default;

    UnownedDeviceResource<Handle, Base> unowned_copy() {
        return UnownedDeviceResource<Handle, Base> { this->m_device, this->m_handle };
    }
};

}