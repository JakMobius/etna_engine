//
// Created by Артем on 10.02.2022.
//

#include "shader-reader.hpp"

#include <string>
#include <fstream>

std::string read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t file_size = (size_t) file.tellg();
    std::string buffer(file_size, '\0');

    file.seekg(0);
    file.read(&buffer[0], file_size);

    file.close();

    return buffer;
}