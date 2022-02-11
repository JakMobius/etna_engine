cmake_minimum_required(VERSION 3.20)
project(vk_test)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)
find_package(glm REQUIRED)
find_package(FreeImage REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O0 -fsanitize=address")

file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")
add_executable(vk_test ${files})
include_directories(${GLFW_INCLUDE_DIR} ${Vulkan_INCLUDE_DIR} ${FREEIMAGE_INCLUDE_DIRS})
target_link_libraries(vk_test ${GLFW_LIBRARY} Vulkan::Vulkan ${GLM_LIBRARIES} ${FREEIMAGE_LIBRARIES})