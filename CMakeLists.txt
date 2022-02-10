cmake_minimum_required(VERSION 3.20)
project(vk_test)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")
add_executable(vk_test ${files})
include_directories(${GLFW_INCLUDE_DIR} ${Vulkan_INCLUDE_DIR})
target_link_libraries(vk_test ${GLFW_LIBRARY} Vulkan::Vulkan)