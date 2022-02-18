#pragma once

#include "vk-device.hpp"
#include "vk-memory.hpp"
#include "vk-image-2d.hpp"

namespace VK {

class MemoryImage2D {
    VK::Memory m_memory;
    VK::Image2D m_image;

public:
    explicit MemoryImage2D(VK::Device* device): m_memory(device), m_image(&m_memory) {

    }

    void destroy() {
        m_image.destroy();
        m_memory.free();
    }

    VK::Memory& get_memory() { return m_memory; }
    VK::Image2D& get_image() { return m_image; }
};

}