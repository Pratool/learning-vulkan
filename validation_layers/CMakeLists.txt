find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(validation_layers
    HelloTriangle.cpp
    main.cpp
    VulkanUtilities.cpp)

target_link_libraries(validation_layers
    glfw
    Vulkan::Vulkan)

install(TARGETS validation_layers
    RUNTIME DESTINATION bin)
