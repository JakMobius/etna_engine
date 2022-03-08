
#include <etna/image-factory.hpp>

Etna::ImageFactory& Etna::ImageFactory::set_image_type(Etna::ImageType image_type) {
    switch(image_type) {
        case ImageType::image_1d:
            m_image_factory.set_image_type(VK_IMAGE_TYPE_1D);
            m_image_view_factory.set_view_type(VK_IMAGE_VIEW_TYPE_1D);
            break;
        case ImageType::image_2d:
            m_image_factory.set_image_type(VK_IMAGE_TYPE_2D);
            m_image_view_factory.set_view_type(VK_IMAGE_VIEW_TYPE_2D);
            break;
        case ImageType::image_3d:
            m_image_factory.set_image_type(VK_IMAGE_TYPE_3D);
            m_image_view_factory.set_view_type(VK_IMAGE_VIEW_TYPE_3D);
            break;
        case ImageType::cube:
            m_image_factory.set_image_type(VK_IMAGE_TYPE_2D);
            m_image_view_factory.set_view_type(VK_IMAGE_VIEW_TYPE_2D);
            break;
        case ImageType::array_1d:
            m_image_factory.set_image_type(VK_IMAGE_TYPE_1D);
            m_image_view_factory.set_view_type(VK_IMAGE_VIEW_TYPE_1D_ARRAY);
            break;
        case ImageType::array_2d:
            m_image_factory.set_image_type(VK_IMAGE_TYPE_2D);
            m_image_view_factory.set_view_type(VK_IMAGE_VIEW_TYPE_2D_ARRAY);
            break;
        case ImageType::array_cube:
            m_image_factory.set_image_type(VK_IMAGE_TYPE_2D);
            m_image_view_factory.set_view_type(VK_IMAGE_VIEW_TYPE_CUBE_ARRAY);

            // TODO: VK_IMAGE_TYPE_2D and VK_IMAGE_VIEW_TYPE_2D_ARRAY can also be used with 3-dimensional images
            // https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkImageViewCreateInfo.html
    }
    return *this;
}
