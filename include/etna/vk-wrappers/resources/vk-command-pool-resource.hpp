#pragma once

namespace VK {
class CommandPoolBase;
}

#include <etna/volk.hpp>
#include <etna/vk-wrappers/device/vk-device.hpp>
#include "vk-device-resource.hpp"

namespace VK {

template<typename Handle>
class CommandPoolResourceBase : public DeviceResourceBase<Handle> {
protected:
    CommandPoolBase* m_command_pool;

    CommandPoolResourceBase(Device* device, CommandPoolBase* command_pool, Handle handle): DeviceResourceBase<Handle>(device, handle), m_command_pool(command_pool) {}
public:

    CommandPoolResourceBase(const CommandPoolResourceBase& copy) = delete;
    CommandPoolResourceBase& operator=(const CommandPoolResourceBase& copy_assign) = delete;
    CommandPoolResourceBase& operator=(CommandPoolResourceBase&& move_assign) noexcept {
        if(&move_assign == this) return *this;
        this->destroy();
        this->m_handle = move_assign.m_handle;
        this->m_device = move_assign.m_device;
        m_command_pool = move_assign.m_command_pool;
        move_assign.m_handle = nullptr;
        return *this;
    }

    virtual ~CommandPoolResourceBase() = default;

    CommandPoolBase* get_command_pool() const { return m_command_pool; };
};

template<typename Handle, typename Base = CommandPoolResourceBase<Handle>>
class UnownedCommandPoolResource : public Base {
public:
    UnownedCommandPoolResource(VK::Device* device, CommandPoolBase* command_pool, Handle handle): Base(device, command_pool, handle) {}
    UnownedCommandPoolResource(): Base(nullptr, nullptr, nullptr) {}
    UnownedCommandPoolResource(UnownedCommandPoolResource&& move) noexcept: Base(move.m_device, move.m_command_pool, move.m_handle) { move.m_handle = nullptr; }
    UnownedCommandPoolResource(const UnownedCommandPoolResource& copy): Base(copy.m_device, copy.m_command_pool, copy.m_handle) {};
    UnownedCommandPoolResource& operator=(UnownedCommandPoolResource&& move_assign) noexcept = default;
    UnownedCommandPoolResource& operator=(const UnownedCommandPoolResource& copy_assign) {
        this->m_handle = copy_assign.m_handle;
        this->m_device = copy_assign.m_device;
        this->m_command_pool = copy_assign.m_command_pool;
        return *this;
    }
};

template<typename Handle, typename Base = CommandPoolResourceBase<Handle>>
class CommandPoolResource : public Base {

public:
    CommandPoolResource(Device* device, CommandPoolBase* pool, Handle handle): Base(device, pool, handle) {}
    CommandPoolResource(): Base(nullptr, nullptr, nullptr) {}
    CommandPoolResource(CommandPoolResource&& move) noexcept: Base(move.m_device, move.m_command_pool, move.m_handle) { move.m_handle = nullptr; }
    CommandPoolResource& operator=(CommandPoolResource&& move_assign) noexcept = default;
    CommandPoolResource& operator=(const CommandPoolResource& copy_assign) = default;

    ~CommandPoolResource() override = default;

    operator UnownedCommandPoolResource<Handle, Base>&() { return as_unowned(); }
    operator const UnownedCommandPoolResource<Handle, Base>&() const { return as_unowned(); }

    const UnownedCommandPoolResource<Handle, Base>& as_unowned() const {
        return reinterpret_cast<UnownedCommandPoolResource<Handle, Base>&>(*this);
    }

    UnownedCommandPoolResource<Handle, Base>& as_unowned() {
        return reinterpret_cast<UnownedCommandPoolResource<Handle, Base>&>(*this);
    }

    UnownedCommandPoolResource<Handle, Base> unowned_copy() const {
        return UnownedCommandPoolResource<Handle, Base> { this->m_device, this->m_command_pool, this->m_handle };
    }
};

}