find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

add_executable(swap_chain
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(swap_chain
    glfw
    spdlog::spdlog
    Vulkan::Vulkan)

install(TARGETS swap_chain
    RUNTIME DESTINATION bin)
