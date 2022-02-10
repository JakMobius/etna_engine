
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <iostream>
#include "app.hpp"

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}