#pragma once

namespace VK {

class CommandPool;
class CommandPoolBase;

}

#include <etna/vk-wrappers/resources/vk-device-resource.hpp>
#include <etna/vk-wrappers/device/vk-device.hpp>
#include "vk-command-buffer.hpp"

namespace VK {

class CommandPoolBase: public DeviceResourceBase<VkCommandPool> {
public:
    using DeviceResourceBase::DeviceResourceBase;
    CommandPoolBase& operator=(CommandPoolBase&& move_assign) = default;

    ~CommandPoolBase() override = default;

    CommandBuffer create_command_buffer(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
};

using UnownedCommandPool = UnownedDeviceResource<VkCommandPool, CommandPoolBase>;

class CommandPool: public DeviceResource<VkCommandPool, CommandPoolBase> {
public:
    using DeviceResource::DeviceResource;
    using DeviceResource::operator=;

    CommandPool(CommandPool&& move) noexcept = default;
    CommandPool& operator=(CommandPool&& move_assign) = default;

    static CommandPool create(VK::Device* device, uint32_t graphics_queue_index, VkCommandPoolCreateFlags flags = 0) {
        VkCommandPoolCreateInfo pool_info {};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.queueFamilyIndex = graphics_queue_index;
        pool_info.flags = flags;

        VkCommandPool command_pool;

        if (vkCreateCommandPool(device->get_handle(), &pool_info, nullptr, &command_pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool");
        }

        return { device, command_pool };
    }

    ~CommandPool() override { destroy(); }

    void destroy() final {
        if(!this->m_handle || !this->m_device) return;
        vkDestroyCommandPool(this->m_device->get_handle(), this->m_handle, nullptr);
        this->m_handle = nullptr;
    }
};

}