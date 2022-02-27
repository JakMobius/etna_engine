#pragma once

#include "../device/vk-device.hpp"
#include "../vk-memory.hpp"
#include "vk-image.hpp"

namespace VK {

class MemoryImage {
    Memory m_memory;
    Image m_image;

public:
    explicit MemoryImage(Image&& image): m_memory(image.get_device()), m_image(std::move(image)) {}
    MemoryImage(MemoryImage&& move) noexcept: m_memory(std::move(move.m_memory)), m_image(std::move(move.m_image)) {}
    MemoryImage& operator=(MemoryImage&& move_assign)  noexcept {
        m_memory = std::move(move_assign.m_memory);
        m_image = std::move(move_assign.m_image);
        return *this;
    }

    MemoryImage(const MemoryImage& copy) = delete;
    MemoryImage& operator=(MemoryImage& copy_assign) = delete;

    ~MemoryImage() {
        destroy();
    }

    void create(VkMemoryPropertyFlags memory_properties) {
        auto device = m_image.get_device();

        VkMemoryRequirements mem_requirements {};
        vkGetImageMemoryRequirements(device->get_handle(), m_image.get_handle(), &mem_requirements);

        auto mem_type = device->get_physical_device()->get_suitable_memory_type(mem_requirements.memoryTypeBits, memory_properties);

        m_memory.set_device(device);
        m_memory.set_size(mem_requirements.size);
        m_memory.set_type(mem_type);
        m_memory.allocate();

        vkBindImageMemory(device->get_handle(), m_image.get_handle(), m_memory.get_handle(), 0);
    }

    void destroy() {
        m_image.destroy();
        m_memory.free();
    }

    VK::Memory& get_memory() { return m_memory; }
    VK::Image& get_image() { return m_image; }
};

}