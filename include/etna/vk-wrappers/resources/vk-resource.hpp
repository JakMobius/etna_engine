#pragma once

namespace VK {

template<typename Handle>
class ResourceBase {
protected:
    Handle m_handle;
    explicit ResourceBase(Handle handle): m_handle(handle) {}
public:

    ResourceBase(const ResourceBase& copy) = delete;
    ResourceBase& operator=(const ResourceBase& copy_assign) = delete;
    ResourceBase& operator=(ResourceBase&& move_assign) noexcept {
        if(&move_assign == this) return *this;
        destroy();
        this->m_handle = move_assign.m_handle;
        move_assign.m_handle = nullptr;
        return *this;
    }

    virtual void destroy() {};
    virtual ~ResourceBase() = default;

    bool is_null() { return !m_handle; }
    bool operator!() { return is_null(); }
    explicit operator bool() { return !is_null(); }

    const Handle& get_handle() const { return m_handle; };
};

template<typename Handle, typename Base = ResourceBase<Handle>>
class UnownedResource : public Base {
public:
    explicit UnownedResource(Handle handle): Base(handle) {}
    UnownedResource(): Base(nullptr) {}
    UnownedResource(UnownedResource&& move) noexcept: Base(move.m_handle) { move.m_handle = nullptr; }
    UnownedResource(const UnownedResource& copy): Base(copy.m_handle) {};
    UnownedResource& operator=(UnownedResource&& move_assign) noexcept = default;
    UnownedResource& operator=(const UnownedResource& copy_assign) {
        this->m_handle = copy_assign.m_handle;
        return *this;
    }
};

template<typename Handle, typename Base = ResourceBase<Handle>>
class Resource : public Base {
public:
    explicit Resource(Handle handle): Base(handle) {}
    Resource(): Base(nullptr) {}
    Resource(Resource&& move) noexcept: Base(move.m_handle) { move.m_handle = nullptr; }
    Resource& operator=(Resource&& move_assign) noexcept = default;
    Resource& operator=(const Resource& copy_assign) = default;

    ~Resource() override = default;

    UnownedResource<Handle, Base> unowned_copy() const {
        return UnownedResource<Handle, Base> { this->m_handle };
    }
};

}