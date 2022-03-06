#pragma once

#include "../command-buffer/vk-command-buffer.hpp"

namespace VK {

class Command {
public:
    virtual ~Command() = default;
    virtual void write(const UnownedCommandBuffer& command_buffer) = 0;
};

}