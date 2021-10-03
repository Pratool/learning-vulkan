find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

add_executable(image_views
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(image_views
    glfw
    spdlog::spdlog
    Vulkan::Vulkan)

install(TARGETS image_views
    RUNTIME DESTINATION bin)
