#pragma once

#include "../../image.hpp"
#include "../../../vulkan/commands/vk-image-blit-command.hpp"
#include "../../../vulkan/barriers/vk-image-memory-barrier.hpp"
#include "../command-queue.hpp"
#include "../command-image.hpp"

namespace Etna {

class CommandGenerateMipmaps {
    CommandImage* m_target = nullptr;

    VkPipelineStageFlags m_source_pipeline_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags m_target_pipeline_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkImageLayout m_target_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    VkAccessFlags m_target_access_mask = VK_ACCESS_SHADER_READ_BIT;
    VkImageAspectFlags m_aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT;

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
        auto vk_image = image->get_image().unowned_copy();

        auto mip_width = (int32_t) extent.width;
        auto mip_height = (int32_t) extent.height;

        VK::ImageMemoryBarrier barrier { vk_image };
        barrier.get_subresource_range().set_aspect_mask(m_aspect_flags);

        for (uint32_t i = 1; i < mip_levels; i++) {

            barrier.get_subresource_range() = VK::ImageSubresourceRange::single_mip_level(mip_levels - 1);
            barrier.set_layouts(image_state.m_layout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
            barrier.set_access_masks(image_state.m_access_masks, VK_ACCESS_TRANSFER_READ_BIT);
            barrier.write(command_buffer->get_buffer(), m_source_pipeline_stage, VK_PIPELINE_STAGE_TRANSFER_BIT);

            VK::ImageBlitCommand blit_command(vk_image, vk_image);
            blit_command.set_source_layout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
            blit_command.set_destination_layout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            blit_command.set_source_mip_level(i - 1);
            blit_command.set_destination_mip_level(i);

            blit_command.get_src_offsets()[0] = { 0, 0, 0 };
            blit_command.get_src_offsets()[1] = { mip_width, mip_height, 1 };

            blit_command.get_dst_offsets()[0] = { 0, 0, 0 };
            blit_command.get_dst_offsets()[1] = { mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1 };

            blit_command.write(command_buffer->get_buffer());

            barrier.set_layouts(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_target_layout);
            barrier.set_access_masks(VK_ACCESS_TRANSFER_READ_BIT, m_target_access_mask);
            barrier.write(command_buffer->get_buffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, m_target_pipeline_stage);

            if (mip_width > 1) mip_width /= 2;
            if (mip_height > 1) mip_height /= 2;
        }

        barrier.get_subresource_range() = VK::ImageSubresourceRange::single_mip_level(mip_levels - 1);
        barrier.set_layouts(image_state.m_layout, m_target_layout);
        barrier.set_access_masks(image_state.m_access_masks, m_target_access_mask);
        barrier.write(command_buffer->get_buffer(), m_source_pipeline_stage, m_target_pipeline_stage);

        image_state.m_layout = m_target_layout;
    }

    CommandGenerateMipmaps& set_target_layout(VkImageLayout target_layout) {
        m_target_layout = target_layout;
        return *this;
    }
    CommandGenerateMipmaps& set_target_access_mask(VkAccessFlags target_access) {
        m_target_access_mask = target_access;
        return *this;
    }
    CommandGenerateMipmaps& set_source_pipeline_stage(VkPipelineStageFlags source_pipeline_stage) {
        m_source_pipeline_stage = source_pipeline_stage;
        return *this;
    }
    CommandGenerateMipmaps& set_target_pipeline_stage(VkPipelineStageFlags target_pipeline_stage) {
        m_target_pipeline_stage = target_pipeline_stage;
        return *this;
    }

    VkImageLayout get_target_layout() const { return m_target_layout; }
    VkAccessFlags get_target_access() const { return m_target_access_mask; }
};

}