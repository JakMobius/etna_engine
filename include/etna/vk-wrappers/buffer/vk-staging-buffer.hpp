#pragma once

#include <span>
#include "vk-memory-buffer.hpp"
#include "vk-buffer-factory.hpp"

namespace VK {

class StagingBufferFactory : public BufferFactory {
public:

    StagingBufferFactory(): BufferFactory() {
        set_usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        set_memory_properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    }

    template<typename T>
    MemoryBuffer create_staging_buffer(Device* device, T data) {
        VkDeviceSize buffer_size = sizeof(data[0]) * data.size();
        return create_staging_buffer(device, data.data(), buffer_size);
    }

    MemoryBuffer create_staging_buffer(Device* device, void* data, size_t size);

};

}