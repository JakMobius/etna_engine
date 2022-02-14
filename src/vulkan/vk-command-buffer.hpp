#pragma once

#include "vk-device.hpp"

namespace VK {

class CommandBuffer {

    Device* m_device;

public:
    CommandBuffer(Device* device): m_device(device) {

    }
};

}