find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

add_shader(shader_modules_fragment_shader shader.frag)
add_shader(shader_modules_vertex_shader shader.vert)

add_executable(shader_modules
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(shader_modules
    glfw
    shader_modules_fragment_shader
    shader_modules_vertex_shader
    spdlog::spdlog
    Vulkan::Vulkan)

install(TARGETS shader_modules
    RUNTIME DESTINATION bin)
