#pragma once

#include <etna/vk-wrappers/device/vk-device.hpp>
#include <etna/vk-wrappers/vk-memory.hpp>
#include "vk-image.hpp"

namespace VK {

class MemoryImage {
    Memory m_memory {};
    Image m_image {};

public:
    MemoryImage() = default;
    explicit MemoryImage(Image&& image): m_memory(image.get_device()), m_image(std::move(image)) {}
    MemoryImage(MemoryImage&& move) noexcept: m_memory(std::move(move.m_memory)), m_image(std::move(move.m_image)) {}
    MemoryImage& operator=(MemoryImage&& move_assign)  noexcept;

    MemoryImage(const MemoryImage& copy) = delete;
    MemoryImage& operator=(MemoryImage& copy_assign) = delete;

    ~MemoryImage() {
        destroy();
    }

    void create(VkMemoryPropertyFlags memory_properties);

    void destroy();

    bool is_null() { return m_memory.is_null(); }

    VK::Memory& get_memory() { return m_memory; }
    VK::Image& get_image() { return m_image; }

    const VK::Memory& get_memory() const { return m_memory; }
    const VK::Image& get_image() const { return m_image; }
};

}