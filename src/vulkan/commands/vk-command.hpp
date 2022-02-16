#pragma once

#include "../vk-command-pool.hpp"

namespace VK {

class Command {
public:

    virtual void write(VK::CommandBuffer* command_buffer) = 0;
};

}