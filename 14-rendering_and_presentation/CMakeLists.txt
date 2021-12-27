find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

add_shader(rendering_and_presentation_fragment_shader shader.frag)
add_shader(rendering_and_presentation_vertex_shader shader.vert)

add_executable(rendering_and_presentation
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(rendering_and_presentation
    glfw
    rendering_and_presentation_fragment_shader
    rendering_and_presentation_vertex_shader
    spdlog::spdlog
    Vulkan::Vulkan)

install(TARGETS rendering_and_presentation
    RUNTIME DESTINATION bin)
