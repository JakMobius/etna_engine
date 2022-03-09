
#include <etna/vk-wrappers/queue/vk-queue.hpp>
#include <etna/vk-wrappers/queue/vk-queue-present-info.hpp>
#include <etna/vk-wrappers/vk-fence.hpp>

VkResult VK::QueueBase::wait_idle() const {
    return vkQueueWaitIdle(m_handle);
}

VkResult VK::QueueBase::present(const VkPresentInfoKHR* present_info) const {
    return vkQueuePresentKHR(m_handle, present_info);
}

VkResult VK::QueueBase::present(VK::QueuePresentInfo &present_info) const {
    return present(&present_info.get_description());
}

VkResult VK::QueueBase::submit(uint32_t submit_count, const VkSubmitInfo* submits, const VK::UnownedFence &fence) const {
    return vkQueueSubmit(m_handle, submit_count, submits, fence.get_handle());
}
