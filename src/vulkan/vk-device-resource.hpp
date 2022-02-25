#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-device.hpp"

namespace VK {

template<typename T>
class DeviceResourceBase {
protected:
    T m_handle;
    Device* m_device;

    DeviceResourceBase(Device* device, T handle): m_handle(handle), m_device(device) {}
public:
    DeviceResourceBase(const DeviceResourceBase& copy) = delete;

    DeviceResourceBase& operator=(DeviceResourceBase& copy_assign) = delete;
    DeviceResourceBase& operator=(DeviceResourceBase&& move_assign) noexcept {
        m_handle = move_assign.m_handle;
        m_device = move_assign.m_device;
        move_assign.m_handle = nullptr;
        return *this;
    }

    virtual ~DeviceResourceBase() = default;

    const T& get_handle() const { return m_handle; };
    Device* get_device() const { return m_device; };
};

template<typename T>
class UnownedDeviceResource : public DeviceResourceBase<T> {
public:
    UnownedDeviceResource(VK::Device* device, T handle): DeviceResourceBase<T>(device, handle) {}
    UnownedDeviceResource(UnownedDeviceResource&& move) noexcept: DeviceResourceBase<T>(move.m_device, move.m_handle) { move.m_handle = nullptr; }
    UnownedDeviceResource(const UnownedDeviceResource& copy): DeviceResourceBase<T>(copy.m_device, copy.m_handle) {};
    UnownedDeviceResource& operator=(const UnownedDeviceResource& copy_assign) {
        this->m_handle = copy_assign.m_handle;
        this->m_device = copy_assign.m_device;
        return *this;
    }
};

template<typename T>
class DeviceResource : public DeviceResourceBase<T> {

public:
    DeviceResource(Device* device, T handle): DeviceResourceBase<T>(device, handle) {}
    DeviceResource(DeviceResource<T>&& move) noexcept: DeviceResourceBase<T>(move.m_device, move.m_handle) { move.m_handle = nullptr; }
    using DeviceResourceBase<T>::operator=;

    ~DeviceResource() override = default;

    UnownedDeviceResource<T> unowned_copy() {
        return UnownedDeviceResource<T> { this->m_device, this->m_handle };
    }
};

}