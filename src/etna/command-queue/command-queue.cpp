//
// Created by Артем on 02.03.2022.
//

#include "command-queue.hpp"
#include "command-image.hpp"

Etna::CommandImage Etna::CommandQueue::provide_image_state(Etna::Image* image, const Etna::ImmediateImageState &state) {
    return {this, image, state};
}
