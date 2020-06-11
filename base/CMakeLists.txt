find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(hellotriangle
    Helpers.cpp
    HelloTriangle.cpp
    main.cpp)

target_include_directories(hellotriangle
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(hellotriangle
    glfw
    Vulkan::Vulkan)

install(TARGETS hellotriangle
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
