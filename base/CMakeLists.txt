find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_shader(hellotriangle_frag shader.frag)
add_shader(hellotriangle_vert shader.vert)

add_executable(hellotriangle
    Helpers.cpp
    HelloTriangle.cpp
    main.cpp)

target_link_libraries(hellotriangle
    Boost::filesystem
    glfw
    hellotriangle_frag
    hellotriangle_vert
    Vulkan::Vulkan)

install(TARGETS hellotriangle
    RUNTIME DESTINATION bin)
