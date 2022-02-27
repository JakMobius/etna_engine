#pragma once

#include "../command-buffer/vk-command-pool.hpp"
#include "../command-buffer/vk-command-buffer.hpp"

namespace VK {

class Command {
public:

    virtual void write(VK::CommandBuffer* command_buffer) = 0;
};

}