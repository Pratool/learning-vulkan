#include "Helpers.hpp"
#include "HelloTriangle.hpp"
#include "hellotriangle_frag/hellotriangle_frag.hpp"
#include "hellotriangle_vert/hellotriangle_vert.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <boost/filesystem.hpp>

#include <algorithm>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>


bool HelloTriangleApplication::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : validationLayers)
    {
        const auto &findIf = std::find_if(std::begin(availableLayers), std::end(availableLayers),
            [layerName](decltype(availableLayers[0]) layerIter)
            {
                return strcmp(layerName, layerIter.layerName) == 0;
            });
        if (findIf == std::end(availableLayers))
        {
            return false;
        }
    }

    return true;
}


SwapChainSupportDetails HelloTriangleApplication::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (0 != formatCount)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if (0 != presentModeCount)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}


VkSurfaceFormatKHR HelloTriangleApplication::chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    // Try to get the format we want, otherwise fallback to the first available one.
    for (const auto &availableFormat : availableFormats)
    {
        if (VK_FORMAT_B8G8R8A8_SRGB == availableFormat.format
            and VK_COLOR_SPACE_SRGB_NONLINEAR_KHR == availableFormat.colorSpace)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}


VkPresentModeKHR HelloTriangleApplication::chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    // Try to select the MAILBOX present mode.
    // Fall back to the guaranteed available FIFO mode.
    for (const auto &availablePresentMode : availablePresentModes)
    {
        if (VK_PRESENT_MODE_MAILBOX_KHR == availablePresentMode)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}


VkExtent2D HelloTriangleApplication::chooseSwapExtent(
    const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (UINT32_MAX != capabilities.currentExtent.width)
    {
        return capabilities.currentExtent;
    }

    VkExtent2D actualExtent = {width_px_, height_px_};
    actualExtent.width = std::max(
        capabilities.minImageExtent.width,
        std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(
        capabilities.minImageExtent.height,
        std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
}


std::vector<const char *> HelloTriangleApplication::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions =
        glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}


void HelloTriangleApplication::createInstance()
{
    if (enableValidationLayers and not checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but unavailable.");
    }

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

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vulkan instance");
    }
}


void HelloTriangleApplication::initVulkan()
{
    if (not verifyExtensions())
    {
        throw std::runtime_error("not all GLFW extensions are available");
    }
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createGraphicsPipeline();
}


void HelloTriangleApplication::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.emplace_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (VK_SUCCESS != vkCreateDevice(physicalDevice, &createInfo, nullptr, &device))
    {
        throw std::runtime_error("Failed to create logical device.");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}


void HelloTriangleApplication::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (0 == deviceCount)
    {
        throw std::runtime_error("Failed to find a GPU with Vulkan support.");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }

    if (VK_NULL_HANDLE == physicalDevice)
    {
        throw std::runtime_error("Failed to find a suitable GPU.");
    }
}


bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice device)
{
    const auto indices = findQueueFamilies(device);
    const auto extensionsSupported = checkDeviceExtensionSupport(device);
    auto swapChainAdequate = false;
    if (extensionsSupported)
    {
        auto swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = not swapChainSupport.formats.empty()
                        and not swapChainSupport.presentModes.empty();
    }
    return indices.isComplete() and extensionsSupported and swapChainAdequate;
}


QueueFamilyIndices HelloTriangleApplication::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    uint32_t qidx = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = qidx;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, qidx, surface, &presentSupport);
        if (presentSupport)
        {
            indices.presentFamily = qidx;
        }

        ++qidx;
    }

    return indices;
}


bool HelloTriangleApplication::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for (const auto &extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}


void HelloTriangleApplication::setupDebugMessenger()
{
    if (not enableValidationLayers)
    {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger)
        != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}


void HelloTriangleApplication::createSurface()
{
    if (VK_SUCCESS != glfwCreateWindowSurface(instance, window, nullptr, &surface))
    {
        throw std::runtime_error("Failed to create window surface.");
    }
}


void HelloTriangleApplication::createSwapChain()
{
    const auto swapChainSupport = querySwapChainSupport(physicalDevice);
    const auto surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    const auto presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    const auto extent = chooseSwapExtent(swapChainSupport.capabilities);
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0
        and imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (VK_SUCCESS != vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain))
    {
        throw std::runtime_error("Failed to create swap chain.");
    }

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}


void HelloTriangleApplication::createImageViews()
{
    swapChainImageViews.resize(swapChainImages.size());
    for (size_t sIdx = 0; sIdx < swapChainImages.size(); ++sIdx)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[sIdx];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (VK_SUCCESS != vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[sIdx]))
        {
            throw std::runtime_error("Failed to create image views.");
        }
    }
}


void HelloTriangleApplication::createGraphicsPipeline()
{
  // Shader is compiled and linked as a std::array.
}


void HelloTriangleApplication::mainLoop()
{
    while (not glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}


void HelloTriangleApplication::cleanup()
{
    for (auto imageView : swapChainImageViews)
    {
        vkDestroyImageView(device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(device, swapChain, nullptr);
    vkDestroyDevice(device, nullptr);
    if (enableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
}
