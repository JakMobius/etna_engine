#pragma once

#include <etna/init.hpp>

namespace Etna {
bool initialize() {
    return volkInitialize() == VK_SUCCESS;
}
}