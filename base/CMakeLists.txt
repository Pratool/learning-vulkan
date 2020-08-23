find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(hellotriangle
    Helpers.cpp
    HelloTriangle.cpp
    main.cpp)

target_link_libraries(hellotriangle
    Boost::filesystem
    glfw
    Vulkan::Vulkan)

install(TARGETS hellotriangle
    RUNTIME DESTINATION bin)
