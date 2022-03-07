cmake_minimum_required(VERSION 3.10)
project(Etna VERSION 0.0.1 DESCRIPTION "Vulkan wrappers for modern C++")

find_package(Vulkan REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O0 -fsanitize=address")

file(GLOB_RECURSE sources "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_library(Etna ${sources})
include_directories(${Vulkan_INCLUDE_DIR})
set_target_properties(Etna PROPERTIES VERSION ${PROJECT_VERSION})

file(GLOB_RECURSE public_headers "${CMAKE_SOURCE_DIR}/src/*.hpp")
set_target_properties(Etna PROPERTIES PUBLIC_HEADER "${public_headers}")
#target_include_directories(Etna PRIVATE .)

include(GNUInstallDirs)
install(TARGETS Etna
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Etna"
        FILES_MATCHING PATTERN "*.hpp"
        )
configure_file(Etna.pc.in Etna.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/Etna.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)