//
// Created by Артем on 09.02.2022.
//

#include "vk-version-code.hpp"

std::ostream &operator<<(std::ostream &stream, const VK::VersionCode &version) {
    stream << version.get_major_version() << "." << version.get_minor_version() << "." << version.get_patch();
    return stream;
}
