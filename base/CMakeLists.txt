find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(hellotriangle
    HelloTriangle.cpp)

target_link_libraries(hellotriangle
    glfw
    Vulkan::Vulkan)

install(TARGETS hellotriangle
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
