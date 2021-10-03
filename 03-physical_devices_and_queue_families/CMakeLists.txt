find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

add_executable(physdevq
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(physdevq
    glfw
    spdlog::spdlog
    Vulkan::Vulkan)

install(TARGETS physdevq
    RUNTIME DESTINATION bin)
