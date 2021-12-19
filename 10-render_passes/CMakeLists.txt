find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

add_shader(render_passes_fragment_shader shader.frag)
add_shader(render_passes_vertex_shader shader.vert)

add_executable(render_passes
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(render_passes
    glfw
    render_passes_fragment_shader
    render_passes_vertex_shader
    spdlog::spdlog
    Vulkan::Vulkan)

install(TARGETS render_passes
    RUNTIME DESTINATION bin)
