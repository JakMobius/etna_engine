
#include <etna/vk-wrappers/buffer/vk-staging-buffer.hpp>

VK::MemoryBuffer VK::StagingBufferFactory::create_staging_buffer(VK::Device* device, void* data, size_t size) {
    set_size(size);
    auto result = create_memory_buffer(device);
    result.get_memory().set_data(data, (size_t) size);
    return result;
}
