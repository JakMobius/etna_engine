cmake_minimum_required(VERSION 3.16)
project(Etna VERSION 0.0.1 DESCRIPTION "Vulkan wrappers for modern C++")

include(GNUInstallDirs)
find_package(Vulkan REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O0 -g3")

file(GLOB_RECURSE sources "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")
add_library(Etna ${sources})
include_directories(${Vulkan_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set_target_properties(Etna PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(Etna PRIVATE "include")

configure_file(etna.pc.in etna.pc @ONLY)
install(TARGETS Etna)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/etna/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/etna")

install(FILES ${CMAKE_BINARY_DIR}/etna.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)