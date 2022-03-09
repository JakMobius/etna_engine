#pragma once

#include <etna/vk-wrappers/command-buffer/vk-command-pool.hpp>
#include <etna/vk-wrappers/queue/vk-queue.hpp>
#include "command-queue.hpp"

namespace Etna {

class CommandQueuePool {

    VK::UnownedCommandPool m_pool;
    VK::Queue m_queue;

public:
    CommandQueuePool(const VK::UnownedCommandPool& pool, const VK::Queue& queue): m_pool(pool), m_queue(queue) {}

    CommandQueue begin_command_queue(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, VkCommandBufferUsageFlags usage = 0) {
        CommandQueue result = create_command_queue(level);
        result.begin(usage);
        return result;
    }

    CommandQueue create_command_queue(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
        CommandQueue result { m_pool.create_command_buffer(level), m_queue };
        return result;
    }
};

}