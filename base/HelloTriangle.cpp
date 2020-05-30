#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>


bool verifyExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::set<std::string> glfwExtensionSet;
    for (size_t idx = 0; idx < glfwExtensionCount; ++idx)
    {
        glfwExtensionSet.insert(std::string(glfwExtensions[idx]));
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    uint32_t glfwExtensionsAvailable = 0;

    std::cout << "Available extensions:" << std::endl;
    for (const auto &extension : extensions)
    {
        uint32_t glfwExtensionCount = glfwExtensionSet.count(extension.extensionName);
        std::cout << '\t' << extension.extensionName << ' ' << glfwExtensionCount << std::endl;
        glfwExtensionsAvailable += glfwExtensionCount >= 1 ? 1 : 0;
    }

    std::cout << "Are all GLFW extensions available? "
              << (glfwExtensionsAvailable == glfwExtensionCount)
              << std::endl;

    return glfwExtensionsAvailable == glfwExtensionCount;
}


class HelloTriangleApplication
{
public:
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow *window;
    // Window size in pixels.
    const uint32_t _width = 800;
    const uint32_t _height = 600;

    VkInstance instance;

    void initWindow()
    {
        glfwInit();

        // Do not create OpenGL context.
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // Disable window resizing.
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(_width, _height, "Vulkan", nullptr, nullptr);
    }

    void createInstance()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create vulkan instance");
        }
    }

    void initVulkan()
    {
        if (not verifyExtensions())
        {
            throw std::runtime_error("not all GLFW extensions are available");
        }
        createInstance();
    }

    void mainLoop()
    {
        while (not glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }

    void cleanup()
    {
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
