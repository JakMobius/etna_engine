
#include <etna/vk-wrappers/codes/vk-object-type-code.hpp>

const char* VK::ObjectTypeCode::name(VkObjectType object_type) {
    switch(object_type) {
        case VK_OBJECT_TYPE_UNKNOWN:                         return "UNKNOWN";
        case VK_OBJECT_TYPE_INSTANCE:                        return "INSTANCE";
        case VK_OBJECT_TYPE_PHYSICAL_DEVICE:                 return "PHYSICAL_DEVICE";
        case VK_OBJECT_TYPE_DEVICE:                          return "DEVICE";
        case VK_OBJECT_TYPE_QUEUE:                           return "QUEUE";
        case VK_OBJECT_TYPE_SEMAPHORE:                       return "SEMAPHORE";
        case VK_OBJECT_TYPE_COMMAND_BUFFER:                  return "COMMAND_BUFFER";
        case VK_OBJECT_TYPE_FENCE:                           return "FENCE";
        case VK_OBJECT_TYPE_DEVICE_MEMORY:                   return "DEVICE_MEMORY";
        case VK_OBJECT_TYPE_BUFFER:                          return "BUFFER";
        case VK_OBJECT_TYPE_IMAGE:                           return "IMAGE";
        case VK_OBJECT_TYPE_EVENT:                           return "EVENT";
        case VK_OBJECT_TYPE_QUERY_POOL:                      return "QUERY_POOL";
        case VK_OBJECT_TYPE_BUFFER_VIEW:                     return "BUFFER_VIEW";
        case VK_OBJECT_TYPE_IMAGE_VIEW:                      return "IMAGE_VIEW";
        case VK_OBJECT_TYPE_SHADER_MODULE:                   return "SHADER_MODULE";
        case VK_OBJECT_TYPE_PIPELINE_CACHE:                  return "PIPELINE_CACHE";
        case VK_OBJECT_TYPE_PIPELINE_LAYOUT:                 return "PIPELINE_LAYOUT";
        case VK_OBJECT_TYPE_RENDER_PASS:                     return "RENDER_PASS";
        case VK_OBJECT_TYPE_PIPELINE:                        return "PIPELINE";
        case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:           return "DESCRIPTOR_SET_LAYOUT";
        case VK_OBJECT_TYPE_SAMPLER:                         return "SAMPLER";
        case VK_OBJECT_TYPE_DESCRIPTOR_POOL:                 return "DESCRIPTOR_POOL";
        case VK_OBJECT_TYPE_DESCRIPTOR_SET:                  return "DESCRIPTOR_SET";
        case VK_OBJECT_TYPE_FRAMEBUFFER:                     return "FRAMEBUFFER";
        case VK_OBJECT_TYPE_COMMAND_POOL:                    return "COMMAND_POOL";
        case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:        return "SAMPLER_YCBCR_CONVERSION";
        case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE:      return "DESCRIPTOR_UPDATE_TEMPLATE";
        case VK_OBJECT_TYPE_SURFACE_KHR:                     return "SURFACE_KHR";
        case VK_OBJECT_TYPE_SWAPCHAIN_KHR:                   return "SWAPCHAIN_KHR";
        case VK_OBJECT_TYPE_DISPLAY_KHR:                     return "DISPLAY_KHR";
        case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:                return "DISPLAY_MODE_KHR";
        case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT:       return "DEBUG_REPORT_CALLBACK_EXT";
        case VK_OBJECT_TYPE_CU_MODULE_NVX:                   return "CU_MODULE_NVX";
        case VK_OBJECT_TYPE_CU_FUNCTION_NVX:                 return "CU_FUNCTION_NVX";
        case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:       return "DEBUG_UTILS_MESSENGER_EXT";
        case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR:      return "ACCELERATION_STRUCTURE_KHR";
        case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT:            return "VALIDATION_CACHE_EXT";
        case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV:       return "ACCELERATION_STRUCTURE_NV";
        case VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL: return "PERFORMANCE_CONFIGURATION_INTEL";
        case VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR:          return "DEFERRED_OPERATION_KHR";
        case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV:     return "INDIRECT_COMMANDS_LAYOUT_NV";
        case VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT:           return "PRIVATE_DATA_SLOT_EXT";
        case VK_OBJECT_TYPE_BUFFER_COLLECTION_FUCHSIA:       return "BUFFER_COLLECTION_FUCHSIA";
        default:                                             return "unknown object";
    }
}

std::ostream &operator<<(std::ostream &stream, const VK::ObjectTypeCode &result) {
    stream << VK::ObjectTypeCode::name(result.get_code());
    return stream;
}
