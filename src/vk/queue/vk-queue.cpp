//
// Created by Артем on 28.02.2022.
//

#include "vk-queue.hpp"
#include "vk-queue-present-info.hpp"
#include "../vk-fence.hpp"

VkResult VK::QueueBase::wait_idle() {
    return vkQueueWaitIdle(m_handle);
}

VkResult VK::QueueBase::present(const VkPresentInfoKHR* present_info) {
    return vkQueuePresentKHR(m_handle, present_info);
}

VkResult VK::QueueBase::present(VK::QueuePresentInfo &present_info) {
    return present(&present_info.get_description());
}

VkResult VK::QueueBase::submit(uint32_t submit_count, const VkSubmitInfo* submits, const VK::Fence &fence) {
    return vkQueueSubmit(m_handle, submit_count, submits, fence.get_handle());
}
