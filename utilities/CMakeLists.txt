find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(app
    main.cpp)

target_link_libraries(app
    glfw
    Vulkan::Vulkan)

install(TARGETS app
    RUNTIME DESTINATION bin)
