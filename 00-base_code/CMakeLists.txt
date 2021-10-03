find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(base_code
    HelloTriangle.cpp
    main.cpp)

target_link_libraries(base_code
    glfw
    Vulkan::Vulkan)

install(TARGETS base_code
    RUNTIME DESTINATION bin)
