#pragma once

#include <unordered_map>
#include "../image.hpp"
#include "../../vulkan/queue/vk-queue.hpp"
#include "../../vulkan/command-buffer/vk-command-buffer.hpp"

namespace Etna {

class CommandImage;
class CommandQueue {

    VK::CommandBuffer* m_buffer;

public:
    CommandQueue(VK::CommandBuffer* buffer): m_buffer(buffer) {}

    CommandImage get_image(Image* image, const ImmediateImageState& state);

    VK::CommandBuffer* get_buffer() { return m_buffer; }

};

}