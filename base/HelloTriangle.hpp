#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <iostream>
#include <cstring>
#include <string>


#ifndef NDEBUG
    constexpr bool enableValidationLayers = true;
#else
    constexpr bool enableValidationLayers = false;
#endif


struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};


struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


class HelloTriangleApplication
{
public:
    explicit HelloTriangleApplication() = default;
    HelloTriangleApplication(const std::string &_appLocation) :
        appLocation{_appLocation}
    {
    }
    HelloTriangleApplication(const HelloTriangleApplication &rhs) = default;
    HelloTriangleApplication(HelloTriangleApplication &&rhs) = default;
    HelloTriangleApplication & operator=(const HelloTriangleApplication &rhs) = default;
    HelloTriangleApplication & operator=(HelloTriangleApplication &&rhs) = default;
    ~HelloTriangleApplication() = default;

    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    std::string appLocation;

    GLFWwindow *window;
    /// Window size in pixels.
    static constexpr uint32_t width_px_ = 800;
    static constexpr uint32_t height_px_ = 600;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation" };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    void initWindow()
    {
        glfwInit();

        // Do not create OpenGL context.
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // Disable window resizing.
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width_px_, height_px_, "Vulkan", nullptr, nullptr);
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {
        std::clog << "[validation layer] " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    /// Checks that Vulkan has validation layers available.
    bool checkValidationLayerSupport();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    std::vector<const char *> getRequiredExtensions();

    void createInstance();
    void initVulkan();

    void createLogicalDevice();

    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    void setupDebugMessenger();

    void createSurface();
    void createSwapChain();
    void createImageViews();
    void createGraphicsPipeline();

    void mainLoop();
    void cleanup();
};
