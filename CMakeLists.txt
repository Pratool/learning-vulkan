# This CMake build system assumes to find dependencies with find_package().
# Read externa/README.md for info on building and installing dependencies.

cmake_minimum_required(VERSION 3.10)

project(LearningVulkan)

add_subdirectory(tools)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/AddShader.cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
# Do not enable these flags yet.
# set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(base)
add_subdirectory(00-base_code)
add_subdirectory(01-instance)
add_subdirectory(02-validation_layers)
add_subdirectory(03-physical_devices_and_queue_families)
add_subdirectory(04-logical_device_and_queues)
add_subdirectory(05-window_surface)
add_subdirectory(06-swap_chain)
add_subdirectory(07-image_views)
add_subdirectory(08-shader_modules)
add_subdirectory(09-fixed_functions)
add_subdirectory(10-render_passes)
add_subdirectory(11-graphics_pipeline)
add_subdirectory(12-framebuffers)
