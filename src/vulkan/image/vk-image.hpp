#pragma once

#include <vulkan/vulkan_core.h>
#include "../vk-memory.hpp"
#include "../vk-command-buffer.hpp"

namespace VK {

class ImageBase {
protected:
    VkImage m_handle;
    VK::Device* m_device;

    ImageBase(VK::Device* device, VkImage handle): m_handle(handle), m_device(device) {}
    ImageBase(const ImageBase& copy) = delete;
public:

    ImageBase& operator=(ImageBase& copy_assign) = delete;
    ImageBase& operator=(ImageBase&& move_assign) noexcept {
        m_handle = move_assign.m_handle;
        m_device = move_assign.m_device;
        move_assign.m_handle = nullptr;
        return *this;
    }

    virtual ~ImageBase() {};

    VkImage get_handle() const { return m_handle; };
    VK::Device* get_device() const { return m_device; };
};

class UnownedImage : public ImageBase {
public:
    UnownedImage(VK::Device* device, VkImage handle): ImageBase(device, handle) {}
    UnownedImage(UnownedImage&& move) noexcept: ImageBase(move.m_device, move.m_handle) { move.m_handle = nullptr; }
    UnownedImage(const UnownedImage& copy): ImageBase(copy.m_device, copy.m_handle) {};
    UnownedImage& operator=(const UnownedImage& copy_assign) {
        m_handle = copy_assign.m_handle;
        m_device = copy_assign.m_device;
        return *this;
    }
};

class Image : public ImageBase {

public:
    Image(Device* device, VkImage handle): ImageBase(device, handle) {}
    Image(Image&& move) noexcept: ImageBase(move.m_device, move.m_handle) { move.m_handle = nullptr; }
    using ImageBase::operator=;

    ~Image() override {
        destroy();
    }

    void destroy() {
        if(!m_handle || !m_device) return;
        vkDestroyImage(m_device->get_handle(), m_handle, nullptr);
        m_handle = nullptr;
    }

    UnownedImage unowned_copy() {
        return UnownedImage { m_device, m_handle };
    }

};

}