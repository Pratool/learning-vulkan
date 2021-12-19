find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

add_shader(fixed_functions_fragment_shader shader.frag)
add_shader(fixed_functions_vertex_shader shader.vert)

add_executable(fixed_functions
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(fixed_functions
    glfw
    fixed_functions_fragment_shader
    fixed_functions_vertex_shader
    spdlog::spdlog
    Vulkan::Vulkan)

install(TARGETS fixed_functions
    RUNTIME DESTINATION bin)
