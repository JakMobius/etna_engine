#pragma once

#include "vk-resource.hpp"
#include <etna/vk-wrappers/instance/vk-instance.hpp>

namespace VK {

template<typename Handle>
class InstanceResourceBase : public ResourceBase<Handle> {
protected:
    Instance* m_instance;

    InstanceResourceBase(Instance* instance, Handle handle): ResourceBase<Handle>(handle), m_instance(instance) {}
public:

    InstanceResourceBase(const InstanceResourceBase& copy) = delete;
    InstanceResourceBase& operator=(const InstanceResourceBase& copy_assign) = delete;
    InstanceResourceBase& operator=(InstanceResourceBase&& move_assign) noexcept {
        if(&move_assign == this) return *this;
        this->destroy();
        this->m_handle = move_assign.m_handle;
        m_instance = move_assign.m_instance;
        move_assign.m_handle = nullptr;
        return *this;
    }

    virtual ~InstanceResourceBase() = default;

    Instance* get_instance() const { return m_instance; };
};

template<typename Handle, typename Base = InstanceResourceBase<Handle>>
class UnownedInstanceResource : public Base {
public:
    UnownedInstanceResource(VK::Instance* instance, Handle handle): Base(instance, handle) {}
    UnownedInstanceResource(): Base(nullptr, nullptr) {}
    UnownedInstanceResource(UnownedInstanceResource&& move) noexcept: Base(move.m_instance, move.m_handle) { move.m_handle = nullptr; }
    UnownedInstanceResource(const UnownedInstanceResource& copy): Base(copy.m_instance, copy.m_handle) {};
    UnownedInstanceResource& operator=(UnownedInstanceResource&& move_assign) noexcept = default;
    UnownedInstanceResource& operator=(const UnownedInstanceResource& copy_assign) {
        this->m_handle = copy_assign.m_handle;
        this->m_instance = copy_assign.m_instance;
        return *this;
    }
};

template<typename Handle, typename Base = InstanceResourceBase<Handle>>
class InstanceResource : public Base {

public:
    InstanceResource(Instance* instance, Handle handle): Base(instance, handle) {}
    InstanceResource(): Base(nullptr, nullptr) {}
    InstanceResource(InstanceResource&& move) noexcept: Base(move.m_instance, move.m_handle) { move.m_handle = nullptr; }
    InstanceResource& operator=(InstanceResource&& move_assign) noexcept = default;
    InstanceResource& operator=(const InstanceResource& copy_assign) = default;

    ~InstanceResource() override = default;

    operator UnownedInstanceResource<Handle, Base>&() { return as_unowned(); }
    operator const UnownedInstanceResource<Handle, Base>&() const { return as_unowned(); }

    const UnownedInstanceResource<Handle, Base>& as_unowned() const {
        return reinterpret_cast<UnownedInstanceResource<Handle, Base>&>(*this);
    }

    UnownedInstanceResource<Handle, Base>& as_unowned() {
        return reinterpret_cast<UnownedInstanceResource<Handle, Base>&>(*this);
    }

    UnownedInstanceResource<Handle, Base> unowned_copy() const {
        return UnownedInstanceResource<Handle, Base> { this->m_device, this->m_handle };
    }
};

}