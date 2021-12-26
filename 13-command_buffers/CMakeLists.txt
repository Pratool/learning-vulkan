find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

add_shader(command_buffers_fragment_shader shader.frag)
add_shader(command_buffers_vertex_shader shader.vert)

add_executable(command_buffers
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(command_buffers
    glfw
    command_buffers_fragment_shader
    command_buffers_vertex_shader
    spdlog::spdlog
    Vulkan::Vulkan)

install(TARGETS command_buffers
    RUNTIME DESTINATION bin)
