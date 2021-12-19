find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

add_shader(graphics_pipeline_fragment_shader shader.frag)
add_shader(graphics_pipeline_vertex_shader shader.vert)

add_executable(graphics_pipeline
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(graphics_pipeline
    glfw
    graphics_pipeline_fragment_shader
    graphics_pipeline_vertex_shader
    spdlog::spdlog
    Vulkan::Vulkan)

install(TARGETS graphics_pipeline
    RUNTIME DESTINATION bin)
