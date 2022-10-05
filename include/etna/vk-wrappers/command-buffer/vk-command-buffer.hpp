#pragma once

namespace VK {

class CommandBufferBase;
class CommandBuffer;

}

#include <span>
#include <etna/vk-wrappers/resources/vk-command-pool-resource.hpp>
#include <etna/vk-wrappers/buffer/vk-buffer.hpp>
#include <etna/vk-wrappers/pipeline/vk-pipeline.hpp>
#include <etna/vk-wrappers/vk-fence.hpp>
#include <etna/vk-wrappers/render-pass/vk-render-pass-begin-info.hpp>

namespace VK {

class CommandBufferBase: public CommandPoolResource<VkCommandBuffer> {
public:
    using CommandPoolResource::CommandPoolResource;
    CommandBufferBase& operator=(CommandBufferBase&& move_assign) = default;

    void reset(VkCommandBufferResetFlags flags = 0);

    void begin(VkCommandBufferUsageFlags flags = 0);

    void end();

    void submit_and_wait(const Queue& queue,
                         const UnownedFence& fence = {},
                         std::span<VkSemaphore> signal_semaphores = {},
                         std::span<VkSemaphore> wait_semaphores = {},
                         std::span<VkPipelineStageFlags> wait_stages = {});

    void submit(const Queue& queue,
                const UnownedFence& fence = {},
                std::span<VkSemaphore> signal_semaphores = {},
                std::span<VkSemaphore> wait_semaphores = {},
                std::span<VkPipelineStageFlags> wait_stages = {});

    void bind_vertex_buffers(std::span<const VkBuffer> buffers, std::span<const VkDeviceSize> offsets);

    void bind_vertex_buffers(uint32_t first_binding, uint32_t binding_count, const VkBuffer* buffers, const VkDeviceSize* offsets);

    void bind_index_buffer(const Buffer& buffer, VkDeviceSize offset, VkIndexType index_type);

    void bind_index_buffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType index_type);

    void bind_descriptor_sets(VkPipelineBindPoint bind_point, VkPipelineLayout layout, uint32_t first_set, std::span<VkDescriptorSet> descriptor_sets, std::span<uint32_t> dynamic_offsets);

    void bind_descriptor_sets(VkPipelineBindPoint bind_point, VkPipelineLayout layout, std::span<VkDescriptorSet> descriptor_sets, std::span<uint32_t> dynamic_offsets);

    void draw_indexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance);

    void draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance);

    void bind_pipeline(VkPipelineBindPoint bind_point, const Pipeline& pipeline);

    void bind_pipeline(VkPipelineBindPoint bind_point, VkPipeline pipeline);

    void begin_render_pass(const RenderPassBeginInfo& begin, VkSubpassContents contents);
    void begin_render_pass(const VkRenderPassBeginInfo* begin, VkSubpassContents contents);

    void end_render_pass();

    ~CommandBufferBase() override = default;
};

using UnownedCommandBuffer = UnownedCommandPoolResource<VkCommandBuffer, CommandBufferBase>;

class CommandBuffer: public CommandPoolResource<VkCommandBuffer, CommandBufferBase> {
public:
    using CommandPoolResource::CommandPoolResource;
    using CommandPoolResource::operator=;

    CommandBuffer(CommandBuffer&& move) noexcept = default;
    CommandBuffer& operator=(CommandBuffer&& move_assign) = default;

    ~CommandBuffer() override { destroy(); }

    void destroy() final;
};

}