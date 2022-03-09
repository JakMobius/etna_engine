#pragma once

#include <etna/image.hpp>
#include <etna/vk-wrappers/commands/vk-image-blit-command.hpp>
#include <etna/vk-wrappers/barriers/vk-image-memory-barrier.hpp>
#include "../command-queue.hpp"
#include "../command-image.hpp"

namespace Etna {

class CommandGenerateMipmaps {
    CommandImage* m_target = nullptr;

    VkPipelineStageFlags m_source_pipeline_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    VkPipelineStageFlags m_target_pipeline_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    ImmediateImageState m_target_image_state = {
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_ACCESS_MEMORY_WRITE_BIT
    };
    VkImageAspectFlags m_aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT;

    void prepare_source_mip_level(Etna::CommandQueue* command_buffer, uint32_t mip_level, VkImageLayout old_layout, VkAccessFlags old_access_mask) {
        // if(state == reading_state) return;

        auto image = m_target->get_etna_image();
        auto& vk_image = image->get_image();

        VK::ImageMemoryBarrier barrier { vk_image };
        barrier.get_subresource_range().set_aspect_mask(m_aspect_flags);
        barrier.get_subresource_range().set_base_mip_level(mip_level);
        barrier.set_layouts(old_layout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        barrier.set_access_masks(old_access_mask, VK_ACCESS_TRANSFER_READ_BIT);
        barrier.write(command_buffer->get_command_buffer(), m_source_pipeline_stage, VK_PIPELINE_STAGE_TRANSFER_BIT);
    }

    void prepare_destination_mip_level(Etna::CommandQueue* command_buffer,
                                       uint32_t mip_level,
                                       VkImageLayout old_layout,
                                       VkAccessFlags old_access_mask,
                                       VkPipelineStageFlags old_state_flags) {
        // if(state == writing_state) return;

        auto image = m_target->get_etna_image();
        auto& vk_image = image->get_image();

        VK::ImageMemoryBarrier barrier { vk_image };
        barrier.get_subresource_range().set_aspect_mask(m_aspect_flags);
        barrier.get_subresource_range().set_base_mip_level(mip_level);
        barrier.set_layouts(old_layout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        barrier.set_access_masks(old_access_mask, VK_ACCESS_TRANSFER_WRITE_BIT);
        barrier.write(command_buffer->get_command_buffer(), old_state_flags, VK_PIPELINE_STAGE_TRANSFER_BIT);
    }

    void blit_level(Etna::CommandQueue* command_buffer, uint32_t mip_level, VkOffset2D size) {
        auto image = m_target->get_etna_image();
        auto& vk_image = image->get_image();

        VK::ImageBlitCommand blit_command(vk_image, vk_image);
        blit_command.set_source_layout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        blit_command.set_destination_layout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        blit_command.get_source_subresource_layers().set_aspect_mask(m_aspect_flags);
        blit_command.get_source_subresource_layers().set_mip_level(mip_level - 1);
        blit_command.get_destination_subresource_layers().set_aspect_mask(m_aspect_flags);
        blit_command.get_destination_subresource_layers().set_mip_level(mip_level);

        blit_command.get_src_offsets()[0] = { 0, 0, 0 };
        blit_command.get_src_offsets()[1] = { size.x, size.y, 1 };

        blit_command.get_dst_offsets()[0] = { 0, 0, 0 };
        blit_command.get_dst_offsets()[1] = { next_mip_dimension(size.x), next_mip_dimension(size.y), 1 };

        blit_command.write(command_buffer->get_command_buffer());
    }

    void bake_level(Etna::CommandQueue* command_buffer, uint32_t mip_level, VkImageLayout old_layout, VkAccessFlags old_access_mask) {
//         if(state == desired_state) return;

        auto image = m_target->get_etna_image();
        auto& vk_image = image->get_image();

        VK::ImageMemoryBarrier barrier { vk_image };
        barrier.get_subresource_range().set_aspect_mask(m_aspect_flags);
        barrier.get_subresource_range().set_base_mip_level(mip_level);
        barrier.set_layouts(old_layout, m_target_image_state.m_layout);
        barrier.set_access_masks(old_access_mask, m_target_image_state.m_access_mask);
        barrier.write(command_buffer->get_command_buffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, m_target_pipeline_stage);
    }

    int32_t next_mip_dimension(int32_t dimension) {
        return dimension > 1 ? dimension / 2 : 1;
    }

public:
    explicit CommandGenerateMipmaps(CommandImage* target): m_target(target) {

    }

    static bool is_supported(VK::Device* device, Image* target) {
        VkFormatProperties format_properties {};
        device->get_physical_device()->get_format_properties(&format_properties, target->get_format());

        return (format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);
    }

    void perform(Etna::CommandQueue* command_buffer) {
        auto image = m_target->get_etna_image();
        auto extent = image->get_extent();
        auto mip_levels = image->get_mip_levels();
        auto image_state = m_target->get_state();
        auto& vk_image = image->get_image();

        VkOffset2D size = { (int32_t) extent.width, (int32_t) extent.height };

        VkAccessFlags original_access_flags = image_state.m_access_mask;
        VkImageLayout original_layout = image_state.m_layout;
        VkPipelineStageFlags original_state = m_source_pipeline_stage;

        VkAccessFlags previous_access_flags = original_access_flags;
        VkImageLayout previous_layout = original_layout;
        VkPipelineStageFlags previous_state = original_state;

        for (uint32_t i = 1; i < mip_levels; i++) {
            prepare_source_mip_level(command_buffer, i - 1, previous_layout, previous_access_flags);
            prepare_destination_mip_level(command_buffer, i, original_layout, original_access_flags, previous_state);
            blit_level(command_buffer, i, size);
            bake_level(command_buffer, i - 1, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_ACCESS_TRANSFER_READ_BIT);

            size.x = next_mip_dimension(size.x);
            size.y = next_mip_dimension(size.y);

            previous_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            previous_access_flags = VK_ACCESS_TRANSFER_WRITE_BIT;
            previous_state = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }

        bake_level(command_buffer, mip_levels - 1, previous_layout, previous_access_flags);

        m_target->get_state() = m_target_image_state;
    }

    CommandGenerateMipmaps& set_source_pipeline_stage(VkPipelineStageFlags source_pipeline_stage) {
        m_source_pipeline_stage = source_pipeline_stage;
        return *this;
    }
    CommandGenerateMipmaps& set_target_pipeline_stage(VkPipelineStageFlags target_pipeline_stage) {
        m_target_pipeline_stage = target_pipeline_stage;
        return *this;
    }

    CommandGenerateMipmaps& set_target_image_state(const Etna::ImmediateImageState& image_state) {
        m_target_image_state = image_state;
        return *this;
    }

    Etna::ImmediateImageState& get_target_image_state() { return m_target_image_state; };
    const Etna::ImmediateImageState& get_target_image_state() const { return m_target_image_state; };
};

}