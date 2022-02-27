//
// Created by Артем on 14.02.2022.
//

#include "vk-surface-context.hpp"
#include "device/vk-device-factory.hpp"

VK::SurfaceContext::SurfaceContext(VK::PhysicalDevice* device, const VK::UnownedSurface& surface) :
        m_physical_device(device),
        m_surface(surface) {

    auto queue_family_indices = m_physical_device->get_queue_family_indices();

    m_graphics_queue_family = queue_family_indices->find_family(VK_QUEUE_GRAPHICS_BIT);
    m_present_queue_family = queue_family_indices->find_surface_present_family(surface);
}


void VK::SurfaceContext::create_command_pool() {

    m_command_pool = VK::CommandPool::create(
        &m_logical_device,
        m_graphics_queue_family,
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
    );
}

void VK::SurfaceContext::create_logical_device(const std::vector<const char*>& extensions,
                                               const std::vector<const char*>& validation_layers) {

    DeviceFactory device_factory;

    device_factory.get_validation_layers() = validation_layers;
    device_factory.get_enabled_extensions() = extensions;

    float queue_priorities[] = { 1.0f };
    for (int queue_family : get_queue_families()) {
        device_factory.add_queue(0, queue_family, queue_priorities);
    }

    device_factory.get_device_features().samplerAnisotropy = VK_TRUE;

    m_logical_device = device_factory.create(m_physical_device);

    m_graphics_queue = m_logical_device.get_queue(m_graphics_queue_family, 0);
    m_present_queue = m_logical_device.get_queue(m_present_queue_family, 0);
}