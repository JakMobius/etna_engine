#pragma once

#include "vk-descriptor.hpp"
#include <etna/vk/image/view/vk-image-view.hpp>
#include <etna/vk/sampler/vk-sampler.hpp>

namespace VK {

class SamplerDescriptor: public Descriptor {
    VkDescriptorImageInfo m_image_info {};

public:
    SamplerDescriptor(const Sampler& sampler, const ImageView& image_view);
};

}