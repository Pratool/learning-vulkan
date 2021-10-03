find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(instance
    HelloTriangle.cpp
    main.cpp)

target_link_libraries(instance
    glfw
    Vulkan::Vulkan)

install(TARGETS instance
    RUNTIME DESTINATION bin)
