find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

add_executable(window_surface
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(window_surface
    glfw
    spdlog::spdlog
    Vulkan::Vulkan)

install(TARGETS window_surface
    RUNTIME DESTINATION bin)
