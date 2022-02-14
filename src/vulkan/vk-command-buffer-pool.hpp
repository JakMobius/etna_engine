#pragma once

#include "vk-device.hpp"

namespace VK {

class CommandBufferPool {

    Device* m_device;

public:
    CommandBufferPool(Device* device): m_device(device) {

    }
};

}