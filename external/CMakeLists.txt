cmake_minimum_required(VERSION 3.10)

project(ExternalProjects)

include(ExternalProject)

externalproject_add(BoostExternalProject
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/boost
    URL "https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.bz2"
    URL_HASH SHA256=59c9b274bc451cf91a9ba1dd2c7fdcaf5d60b1b3aa83f2c9fa143417cc660722
    DOWNLOAD_NO_PROGRESS ON
    BUILD_IN_SOURCE ON
    CONFIGURE_COMMAND ./bootstrap.sh --prefix=${CMAKE_INSTALL_PREFIX}
    BUILD_COMMAND ./b2 install
        --prefix=${CMAKE_INSTALL_PREFIX}
        variant=release
        runtime-link=shared
    INSTALL_COMMAND "")

externalproject_add(GLFWExternalProject
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glfw
    URL "https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.zip"
    URL_HASH SHA256=08a33a512f29d7dbf78eab39bd7858576adcc95228c9efe8e4bc5f0f3261efc7
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_PREFIX_PATH:PATH=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX})

externalproject_add(GLMExternalProject
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glm
    URL "https://github.com/g-truc/glm/archive/0.9.9.8.tar.gz"
    URL_HASH SHA256=7d508ab72cb5d43227a3711420f06ff99b0a0cb63ee2f93631b162bfe1fe9592
    DOWNLOAD_NO_PROGRESS ON
    BUILD_IN_SOURCE ON
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND pwd && cp -R cmake/glm ${CMAKE_INSTALL_PREFIX}/cmake/
    COMMAND cp -R glm ${CMAKE_INSTALL_PREFIX}/include/
    COMMAND rm ${CMAKE_INSTALL_PREFIX}/include/glm/CMakeLists.txt)

externalproject_add(SpdlogExternalProject
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/spdlog
    URL "https://github.com/gabime/spdlog/archive/refs/tags/v1.9.2.tar.gz"
    URL_HASH SHA256=6fff9215f5cb81760be4cc16d033526d1080427d236e86d70bb02994f85e3d38
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_PREFIX_PATH:PATH=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX})
