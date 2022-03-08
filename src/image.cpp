
#include <etna/image.hpp>
#include <etna/image-factory.hpp>

Etna::Image::Image(const Etna::ImageFactory &factory, VK::Device* device) {
    auto& image_factory = factory.get_image_factory();
    auto& image_view_factory = factory.get_image_view_factory();

    m_image = image_factory.create(device);
    m_aspect_mask = factory.get_aspect_mask();
    m_format = factory.get_format();
    m_tiling = factory.get_tiling();
    m_mip_levels = factory.get_mip_levels();
    m_array_layers = factory.get_array_layers();
    m_extent = factory.get_extent();

    m_image_view = image_view_factory.create(device, m_image.get_image());
}
