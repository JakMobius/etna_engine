#pragma once

#include "../vulkan/image/vk-image.hpp"
#include "../vulkan/image/vk-memory-image.hpp"
#include "../vulkan/image/view/vk-image-view.hpp"

namespace Etna {

class ImageFactory;

struct ImmediateImageState {
    VkImageLayout m_layout {};
    VkAccessFlags m_access_masks {};
};

class Image {
    VK::MemoryImage m_image {};
    VK::ImageView m_image_view {};
    VkExtent3D m_extent {};
    VkFormat m_format {};
    VkImageTiling m_tiling {};
    VkImageAspectFlags m_aspect_mask {};
    uint32_t m_mip_levels {};

public:

    Image(const Etna::ImageFactory &factory, VK::Device* device);

    VK::Device* get_device()                   { return m_image.get_memory().get_device(); }
    VK::Image& get_image()                     { return m_image.get_image(); }
    VK::Memory& get_memory()                   { return m_image.get_memory(); }
    VK::ImageView& get_view()                  { return m_image_view; }


    const VK::Image& get_image()                     const { return m_image.get_image(); }
    const VK::Memory& get_memory()                   const { return m_image.get_memory(); }
    const VK::ImageView& get_image_view()            const { return m_image_view; }
    VkExtent3D get_extent()                          const { return m_extent; }
    VkFormat get_format()                            const { return m_format; }
    VkImageTiling get_tiling()                       const { return m_tiling; }
    uint32_t get_mip_levels()                        const { return m_mip_levels; }
    VkImageAspectFlags get_aspect_mask()             const { return m_aspect_mask; }

    void destroy() {
        m_image.destroy();
        m_image_view.destroy();
    }
};


}