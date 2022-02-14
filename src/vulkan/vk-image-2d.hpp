#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-memory.hpp"
#include "vk-image-configuration.hpp"
#include "vk-command-buffer.hpp"

namespace VK {

class Image2D : public ImageConfiguration {

    Memory* m_memory;

public:
    explicit Image2D(Memory* memory): m_memory(memory) {}
    ~Image2D() {
        destroy();
    }

    void create();

    void destroy() {
        if(m_handle) {
            vkDestroyImage(m_memory->get_device()->get_handle(), m_handle, nullptr);
            m_handle = nullptr;
        }
    }

    void perform_layout_transition(VK::CommandBuffer& command_buffer, VkImageLayout old_layout, VkImageLayout new_layout);
};

}