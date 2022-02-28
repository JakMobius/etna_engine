#pragma once

namespace VK {
class QueuePresentInfo;
}

#include <vulkan/vulkan_core.h>
#include "../resources/vk-resource.hpp"
#include "../vk-fence.hpp"

namespace VK {

class QueueBase : public ResourceBase<VkQueue> {
public:
    using ResourceBase::ResourceBase;
    QueueBase& operator=(QueueBase&& move_assign) = default;

    VkResult wait_idle();

    VkResult present(const VkPresentInfoKHR* present_info);

    VkResult present(QueuePresentInfo& present_info);

    VkResult submit(uint32_t submit_count, const VkSubmitInfo* submits, const Fence& fence = {});

    ~QueueBase() override = default;
};

using Queue = UnownedResource<VkQueue, QueueBase>;

}