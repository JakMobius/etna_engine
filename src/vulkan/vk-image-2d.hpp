#pragma once

#include <vulkan/vulkan_core.h>
#include "vk-memory.hpp"
#include "vk-base-image.hpp"
#include "vk-command-buffer.hpp"

namespace VK {

class Image2D : public BaseImage {

public:
    explicit Image2D(Memory* memory): BaseImage() {
        m_memory = memory;
        m_image_type = VK_IMAGE_TYPE_2D;
    }
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