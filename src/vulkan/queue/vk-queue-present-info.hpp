#pragma once

#include <vulkan/vulkan_core.h>
#include <span>

namespace VK {

class QueuePresentInfo {
    VkPresentInfoKHR m_description = {};
public:
    QueuePresentInfo() {
        m_description.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    }

    void set_swapchains(std::span<VkSwapchainKHR> swapchains) {
        m_description.swapchainCount = swapchains.size();
        m_description.pSwapchains = swapchains.data();
    }

    void set_wait_semaphores(std::span<VkSemaphore> semaphores) {
        m_description.waitSemaphoreCount = semaphores.size();
        m_description.pWaitSemaphores = semaphores.data();
    }

    void set_results(VkResult* results) {
        m_description.pResults = results;
    }

    void set_images(const uint32_t* image_indices) {
        m_description.pImageIndices = image_indices;
    }

    VkPresentInfoKHR& get_description() { return m_description; }
};

}