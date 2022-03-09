
#include <etna/vk-wrappers/pipeline/vk-pipeline-viewport-state.hpp>

VkPipelineViewportStateCreateInfo VK::PipelineViewportState::compile() const {
    VkPipelineViewportStateCreateInfo viewport_state {};
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = get_viewports().size();
    viewport_state.pViewports = get_viewports().data();
    viewport_state.scissorCount = get_scissors().size();
    viewport_state.pScissors = get_scissors().data();
    return viewport_state;
}
