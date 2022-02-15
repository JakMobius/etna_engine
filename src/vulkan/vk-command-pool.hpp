#pragma once

namespace VK {

class CommandPool;

}

#include "vk-surface-context.hpp"
#include "vk-device.hpp"
#include "vk-command-buffer.hpp"

namespace VK {

class CommandPool {

    VK::SurfaceContext* m_surface_context;
    VkCommandPool m_handle;

public:
    CommandPool(VkCommandPool command_pool, VK::SurfaceContext* surface_context):
        m_handle(command_pool), m_surface_context(surface_context) {

    }

    ~CommandPool();

    VK::CommandBuffer create_command_buffer();

    VkCommandPool get_handle() { return m_handle; }

    VK::SurfaceContext* get_surface_context() { return m_surface_context; };
};

}