#pragma once

#include <etna/image.hpp>
#include "command-queue.hpp"

namespace Etna {

class CommandImage {
    friend class CommandQueue;
    CommandQueue* m_queue;
    Image* m_image;
    ImmediateImageState m_state;

    CommandImage(CommandQueue* queue, Image* image, const ImmediateImageState& state): m_queue(queue), m_image(image), m_state(state) {};
public:

    CommandQueue* get_queue() { return m_queue; }
    Image* get_etna_image() { return m_image; }
    ImmediateImageState& get_state() { return m_state; }
};

}