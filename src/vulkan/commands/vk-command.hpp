#pragma once

namespace VK {

class CommandBuffer;
class Command {
public:
    virtual ~Command() = default;
    virtual void write(CommandBuffer* command_buffer) = 0;
};

}