#pragma once

#include "../../image.hpp"
#include "../command-image.hpp"
#include "../../../vulkan/barriers/vk-image-memory-barrier.hpp"

namespace Etna {

class CommandImageLayoutConvert {

    VkPipelineStageFlags m_source_pipeline_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags m_target_pipeline_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkAccessFlags m_target_access_mask = INT_MAX;
    VkImageLayout m_target_layout = (VkImageLayout)INT_MAX;
    VK::ImageSubresourceRange m_subresource_range {};
    CommandImage* m_image;

public:
    explicit CommandImageLayoutConvert(CommandImage* image): m_image(image) {

    }

    CommandImageLayoutConvert& set_target_layout(VkImageLayout target_layout) {
        m_target_layout = target_layout;
        return *this;
    }

    CommandImageLayoutConvert& set_target_access_mask(VkAccessFlags target_access) {
        m_target_access_mask = target_access;
        return *this;
    }

    CommandImageLayoutConvert& set_subresource_range(VK::ImageSubresourceRange& subresource_range) {
        m_subresource_range = subresource_range;
        return *this;
    }

    CommandImageLayoutConvert& set_source_pipeline_stage(VkPipelineStageFlags source_pipeline_stage) {
        m_source_pipeline_stage = source_pipeline_stage;
        return *this;
    }

    CommandImageLayoutConvert& set_target_pipeline_stage(VkPipelineStageFlags target_pipeline_stage) {
        m_target_pipeline_stage = target_pipeline_stage;
        return *this;
    }

    void perform(VK::CommandBuffer* command_buffer) {

        VkAccessFlags src_access = m_image->get_state().m_access_masks;
        VkImageLayout src_layout = m_image->get_state().m_layout;

        VkAccessFlags target_access = m_target_access_mask;
        VkImageLayout target_layout = m_target_layout;

        if(target_access == INT_MAX) target_access = src_access;
        if(target_layout == INT_MAX) target_layout = src_layout;

        VK::ImageMemoryBarrier layout_conversion_barrier { m_image->get_etna_image()->get_image().unowned_copy() };
        layout_conversion_barrier.set_layouts(src_layout, target_layout);
        layout_conversion_barrier.set_access_masks(src_access, target_access);
        layout_conversion_barrier.get_subresource_range() = m_subresource_range;
        layout_conversion_barrier.write(command_buffer, m_source_pipeline_stage, m_target_pipeline_stage);

        m_image->get_state().m_access_masks = target_access;
        m_image->get_state().m_layout = target_layout;
    }

    VK::ImageSubresourceRange& get_subresource_range() { return m_subresource_range; }
    const VK::ImageSubresourceRange& get_subresource_range() const { return m_subresource_range; }

    VkImageLayout get_target_layout() const { return m_target_layout; }
    VkAccessFlags get_target_access_mask() const { return m_target_access_mask; }
};

}