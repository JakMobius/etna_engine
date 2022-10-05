#pragma once

#include <climits>
#include <etna/image.hpp>
#include "../command-image.hpp"
#include <etna/vk-wrappers/barriers/vk-image-memory-barrier.hpp>

namespace Etna {

class CommandImageBarrier {

    VkPipelineStageFlags m_source_pipeline_stage = 0;
    VkPipelineStageFlags m_target_pipeline_stage = 0;
    VkAccessFlags m_target_access_mask = INT_MAX;
    VkImageLayout m_target_layout = (VkImageLayout)INT_MAX;
    VK::ImageSubresourceRange m_subresource_range {};
    CommandImage* m_image;

public:
    explicit CommandImageBarrier(CommandImage* image): m_image(image) {

    }

    CommandImageBarrier& set_target_state(const ImmediateImageState& target_state) {
        set_target_layout(target_state.m_layout);
        set_target_access_mask(target_state.m_access_mask);
        return *this;
    }

    CommandImageBarrier& set_target_layout(VkImageLayout target_layout) {
        m_target_layout = target_layout;
        return *this;
    }

    CommandImageBarrier& set_target_access_mask(VkAccessFlags target_access) {
        m_target_access_mask = target_access;
        return *this;
    }

    CommandImageBarrier& set_subresource_range(const VK::ImageSubresourceRange& subresource_range) {
        m_subresource_range = subresource_range;
        return *this;
    }

    CommandImageBarrier& set_source_pipeline_stage(VkPipelineStageFlags source_pipeline_stage) {
        m_source_pipeline_stage = source_pipeline_stage;
        return *this;
    }

    CommandImageBarrier& set_target_pipeline_stage(VkPipelineStageFlags target_pipeline_stage) {
        m_target_pipeline_stage = target_pipeline_stage;
        return *this;
    }

    void perform(Etna::CommandQueue* command_queue) {

        VkAccessFlags src_access = m_image->get_state().m_access_mask;
        VkImageLayout src_layout = m_image->get_state().m_layout;

        VkAccessFlags target_access = m_target_access_mask;
        VkImageLayout target_layout = m_target_layout;

        if(target_access == INT_MAX) target_access = src_access;
        if(target_layout == INT_MAX) target_layout = src_layout;

        VK::ImageMemoryBarrier layout_conversion_barrier { m_image->get_etna_image()->get_image() };
        layout_conversion_barrier.set_layouts(src_layout, target_layout);
        layout_conversion_barrier.set_access_masks(src_access, target_access);
        layout_conversion_barrier.get_subresource_range() = m_subresource_range;
        layout_conversion_barrier.write(command_queue->get_command_buffer(), m_source_pipeline_stage, m_target_pipeline_stage);

        m_image->get_state().m_access_mask = target_access;
        m_image->get_state().m_layout = target_layout;
    }

    VK::ImageSubresourceRange& get_subresource_range() { return m_subresource_range; }
    const VK::ImageSubresourceRange& get_subresource_range() const { return m_subresource_range; }

    VkImageLayout get_target_layout() const { return m_target_layout; }
    VkAccessFlags get_target_access_mask() const { return m_target_access_mask; }
};

}