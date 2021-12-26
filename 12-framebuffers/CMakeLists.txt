find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

add_shader(framebuffers_fragment_shader shader.frag)
add_shader(framebuffers_vertex_shader shader.vert)

add_executable(framebuffers
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(framebuffers
    glfw
    framebuffers_fragment_shader
    framebuffers_vertex_shader
    spdlog::spdlog
    Vulkan::Vulkan)

install(TARGETS framebuffers
    RUNTIME DESTINATION bin)
