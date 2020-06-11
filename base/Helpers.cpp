#include "Helpers.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <set>
#include <vector>


/**
 * \func verifyExtensions
 *
 * Tests that all extensions required by GLFW are available,
 * returning true if so, and false otherwise.
 **/
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


/**
 * The two following functions call to Vulkan extensions.
 * To do so vkGetInstanceProcAddr will acquire the corresponding function pointer,
 * or a nullptr if the extension cannot be loaded.
 **/
VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (nullptr != func)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}


void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (nullptr != func)
    {
        func(instance, debugMessenger, pAllocator);
    }
}
