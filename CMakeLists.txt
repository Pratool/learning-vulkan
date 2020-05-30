# This CMake build system assumes to find dependencies with find_package().
# Read externa/README.md for info on building and installing dependencies.

cmake_minimum_required(VERSION 3.10)

project(LearningVulkan)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
# Do not enable these flags yet.
# set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(app
    main.cpp)

target_link_libraries(app
    glfw
    vulkan)

install(TARGETS app
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
