#pragma once

#include <etna/volk.hpp>
#include <span>

namespace VK {

class QueuePresentInfo {
    VkPresentInfoKHR m_description = {};
public:
    QueuePresentInfo() {
        m_description.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    }

    QueuePresentInfo& set_swapchains(std::span<VkSwapchainKHR> swapchains) {
        m_description.swapchainCount = swapchains.size();
        m_description.pSwapchains = swapchains.data();
        return *this;
    }

    QueuePresentInfo& set_wait_semaphores(std::span<VkSemaphore> semaphores) {
        m_description.waitSemaphoreCount = semaphores.size();
        m_description.pWaitSemaphores = semaphores.data();
        return *this;
    }

    QueuePresentInfo& set_results(VkResult* results) {
        m_description.pResults = results;
        return *this;
    }

    QueuePresentInfo& set_images(const uint32_t* image_indices) {
        m_description.pImageIndices = image_indices;
        return *this;
    }

    VkPresentInfoKHR& get_description() { return m_description; }
};

}