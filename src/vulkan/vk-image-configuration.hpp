#pragma once

#include <vulkan/vulkan_core.h>

namespace VK {

class ImageConfiguration {

protected:
    VkImage m_handle = nullptr;
    VkExtent2D m_extent = {0, 0};
    VkImageCreateFlags m_flags = 0;
    VkFormat m_format = VK_FORMAT_MAX_ENUM;
    uint32_t m_mip_levels = 1;
    uint32_t m_array_layers = 1;
    VkSampleCountFlagBits m_samples = VK_SAMPLE_COUNT_1_BIT;
    VkImageTiling m_tiling = VK_IMAGE_TILING_OPTIMAL;
    VkImageUsageFlags m_usage = 0;
    VkImageLayout m_initial_layout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkMemoryPropertyFlags m_memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    VkSharingMode m_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
    std::vector<uint32_t> m_queue_family_indices {};


    static void report_illegal_state_change() {
        throw std::runtime_error("cannot change VK::Image internal state while it is created");
    }

    ImageConfiguration() = default;

public:

    VkImage get_handle() { return m_handle; }

    void set_size(uint32_t width, uint32_t height) {
        if(m_handle) report_illegal_state_change();
        m_extent = { width, height };
    }

    void set_flags(VkImageCreateFlags flags) {
        if(m_handle) report_illegal_state_change();
        m_flags = flags;
    }

    void set_format(VkFormat format) {
        if(m_handle) report_illegal_state_change();
        m_format = format;
    }

    void set_mip_levels(uint32_t mip_levels) {
        if(m_handle) report_illegal_state_change();
        m_mip_levels = mip_levels;
    }

    void set_array_layers(uint32_t array_layers) {
        if(m_handle) report_illegal_state_change();
        m_array_layers = array_layers;
    }

    void set_samples(VkSampleCountFlagBits samples) {
        if(m_handle) report_illegal_state_change();
        m_samples = samples;
    }

    void set_tiling(VkImageTiling tiling) {
        if(m_handle) report_illegal_state_change();
        m_tiling = tiling;
    }

    void set_usage(VkBufferUsageFlags usage) {
        if(m_handle) report_illegal_state_change();
        m_usage = usage;
    }

    void set_initial_layout(VkImageLayout initial_layout) {
        if(m_handle) report_illegal_state_change();
        m_initial_layout = initial_layout;
    }

    void set_sharing_mode(VkSharingMode sharing_mode) {
        if(m_handle) report_illegal_state_change();
        m_sharing_mode = sharing_mode;
    }

    void set_memory_properties(VkMemoryPropertyFlags memory_properties) {
        if(m_handle) report_illegal_state_change();
        m_memory_properties = memory_properties;
    }

    VkExtent2D get_size()                         const { return m_extent; }
    VkImageCreateFlags get_flags()                const { return m_flags; }
    VkFormat get_format()                         const { return m_format; }
    uint32_t get_mip_levels()                     const { return m_mip_levels; }
    uint32_t get_array_layers()                   const { return m_array_layers; }
    VkSampleCountFlagBits get_samples()           const { return m_samples; }
    VkImageTiling get_tiling()                    const { return m_tiling; }
    VkBufferUsageFlags get_usage()                const { return m_flags; }
    VkImageLayout get_initial_layout()            const { return m_initial_layout; }
    VkSharingMode get_sharing_mode()              const { return m_sharing_mode; }
    VkMemoryPropertyFlags get_memory_properties() const { return m_memory_properties; }

    std::vector<uint32_t>& get_queue_family_indices() {
        return m_queue_family_indices;
    }
};

}