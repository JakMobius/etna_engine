//
// Created by Артем on 02.03.2022.
//

#include "etna/command-queue/command-queue.hpp"
#include "etna/command-queue/command-image.hpp"

Etna::CommandImage Etna::CommandQueue::provide_image_state(Etna::Image* image, const Etna::ImmediateImageState &state) {
    return {this, image, state};
}
