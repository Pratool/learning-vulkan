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

install(DIRECTORY shaders
        DESTINATION ${CMAKE_INSTALL_PREFIX})

add_subdirectory(base)
add_subdirectory(base_code)
add_subdirectory(image_views)
add_subdirectory(instance)
add_subdirectory(logical_device_and_queues)
add_subdirectory(physical_devices_and_queue_families)
add_subdirectory(swap_chain)
add_subdirectory(utilities)
add_subdirectory(validation_layers)
add_subdirectory(window_surface)
