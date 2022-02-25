#pragma once

#include "buffer/vk-memory-buffer.hpp"
#include "buffer/vk-buffer-factory.hpp"

namespace VK {

class StagingBufferFactory : public BufferFactory {
public:

    StagingBufferFactory(): BufferFactory() {
        set_usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        set_memory_properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    }

    template<typename T>
    MemoryBuffer create_staging_buffer(Device* device, std::vector<T> data) {
        VkDeviceSize buffer_size = sizeof(data[0]) * data.size();
        set_size(buffer_size);
        auto result = create_memory_buffer(device);
        result.get_memory().set_data(data.data(), (size_t) buffer_size);
        return result;
    }

    MemoryBuffer create_staging_buffer(Device* device, void* data, size_t size) {
        set_size(size);
        auto result = create_memory_buffer(device);
        result.get_memory().set_data(data, (size_t) size);
        return result;
    }

};

}