cmake_minimum_required(VERSION 3.10)

project(ExternalProjects)

include(ExternalProject)

externalproject_add(GLFWExternalProject
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glfw
    URL "https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.zip"
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_PREFIX_PATH:PATH=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX})
