//
// Created by Артем on 02.03.2022.
//

#include "command-queue.hpp"
#include "command-image.hpp"

Etna::CommandImage Etna::CommandQueue::get_image(Etna::Image* image, const Etna::ImmediateImageState &state) {
    return {this, image, state};
}
