#pragma once

namespace VK {
class QueuePresentInfo;
}

#include <etna/volk.hpp>
#include <etna/vk-wrappers/resources/vk-resource.hpp>
#include <etna/vk-wrappers/vk-fence.hpp>

namespace VK {

class QueueBase : public ResourceBase<VkQueue> {
public:
    using ResourceBase::ResourceBase;
    QueueBase& operator=(QueueBase&& move_assign) = default;

    VkResult wait_idle() const;

    VkResult present(const VkPresentInfoKHR* present_info) const;

    VkResult present(QueuePresentInfo& present_info) const;

    VkResult submit(uint32_t submit_count, const VkSubmitInfo* submits, const UnownedFence& fence = {}) const;

    ~QueueBase() override = default;
};

using Queue = UnownedResource<VkQueue, QueueBase>;

}