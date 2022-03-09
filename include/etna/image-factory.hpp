#pragma once

#include <etna/vk-wrappers/image/vk-image.hpp>
#include <etna/vk-wrappers/image/vk-image-factory.hpp>
#include <etna/vk-wrappers/image/view/vk-image-view-factory.hpp>
#include <etna/vk-wrappers/image/vk-image-subresource-range.hpp>
#include <etna/image.hpp>

namespace Etna {

enum class ImageType {
    image_1d,
    image_2d,
    image_3d,
    cube,
    array_1d,
    array_2d,
    array_cube
};

class ImageFactory {
    VK::ImageFactory m_image_factory {};
    VK::ImageViewFactory m_image_view_factory {};
    ImageType m_image_type = ImageType::image_2d;

public:
    ImageFactory() {
        set_aspect_mask(VK_IMAGE_ASPECT_COLOR_BIT);
    }

    ImageFactory& set_image_type(ImageType image_type);

    ImageFactory& set_image_flags(VkImageCreateFlags flags) {
        m_image_factory.set_flags(flags);
        return *this;
    }

    ImageFactory& set_image_view_flags(VkImageViewCreateFlags flags) {
        m_image_view_factory.set_flags(flags);
        return *this;
    }

    ImageFactory& set_format(VkFormat format) {
        m_image_factory.set_format(format);
        m_image_view_factory.set_format(format);
        return *this;
    }

    ImageFactory& set_mip_levels(uint32_t mip_levels) {
        m_image_factory.set_mip_levels(mip_levels);
        m_image_view_factory.get_subresource_range().set_mip_levels(mip_levels);
        return *this;
    }

    ImageFactory& set_base_mip_level(uint32_t base_mip_level) {
        m_image_view_factory.get_subresource_range().set_base_mip_level(base_mip_level);
        return *this;
    }

    ImageFactory& set_array_layers(uint32_t array_layers) {
        m_image_factory.set_array_layers(array_layers);
        m_image_view_factory.get_subresource_range().set_array_layers(array_layers);
        return *this;
    }

    ImageFactory& set_base_array_layer(uint32_t base_array_layer) {
        m_image_view_factory.get_subresource_range().set_base_array_layer(base_array_layer);
        return *this;
    }

    ImageFactory& set_aspect_mask(VkImageAspectFlags aspect_mask) {
        m_image_view_factory.get_subresource_range().set_aspect_mask(aspect_mask);
        return *this;
    }

    ImageFactory& set_samples(VkSampleCountFlagBits samples) {
        m_image_factory.set_samples(samples);
        return *this;
    }

    ImageFactory& set_tiling(VkImageTiling tiling) {
        m_image_factory.set_tiling(tiling);
        return *this;
    }

    ImageFactory& set_usage(VkBufferUsageFlags usage) {
        m_image_factory.set_usage(usage);
        return *this;
    }

    ImageFactory& set_initial_layout(VkImageLayout initial_layout) {
        m_image_factory.set_initial_layout(initial_layout);
        return *this;
    }

    ImageFactory& set_sharing_mode(VkSharingMode sharing_mode) {
        m_image_factory.set_sharing_mode(sharing_mode);
        return *this;
    }

    ImageFactory& set_memory_properties(VkMemoryPropertyFlags memory_properties) {
        m_image_factory.set_memory_properties(memory_properties);
        return *this;
    }

    ImageFactory& set_extent(VkExtent3D extent) {
        m_image_factory.set_extent(extent);
        return *this;
    }

    ImageType get_image_type()                      const { return m_image_type; }
    VkImageCreateFlags get_image_flags()            const { return m_image_factory.get_flags(); }
    VkImageViewCreateFlags get_image_view_flags()   const { return m_image_view_factory.get_flags(); }
    const VkFormat get_format()                     const { return m_image_factory.get_format(); }
    const unsigned int get_mip_levels()             const { return m_image_factory.get_mip_levels(); }
    const unsigned int get_array_layers()           const { return m_image_factory.get_array_layers(); }
    const VkSampleCountFlagBits get_samples()       const { return m_image_factory.get_samples(); }
    const VkImageTiling get_tiling()                const { return m_image_factory.get_tiling(); }
    const unsigned int get_usage()                  const { return m_image_factory.get_usage(); }
    const VkImageLayout get_initial_layout()        const { return m_image_factory.get_initial_layout(); }
    const VkSharingMode get_sharing_mode()          const { return m_image_factory.get_sharing_mode(); }
    const unsigned int get_memory_properties()      const { return m_image_factory.get_memory_properties(); }
    const VkExtent3D get_extent()                   const { return m_image_factory.get_extent(); }
    VkImageAspectFlags get_aspect_mask()            const { return m_image_view_factory.get_subresource_range().get_aspect_mask(); }

    const VK::ImageFactory& get_image_factory() const { return m_image_factory; }
    const VK::ImageViewFactory& get_image_view_factory() const { return m_image_view_factory; }

    Etna::ImmediateImageState get_initial_image_state() const {
        return { get_initial_layout(), 0 };
    }
};

}