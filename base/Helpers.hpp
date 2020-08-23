#pragma once

// external library headers
#include <vulkan/vulkan_core.h>

// standard template library headers
#include <vector>
#include <fstream>


/**
 * \func verifyExtensions
 *
 * Tests that all extensions required by GLFW are available,
 * returning true if so, and false otherwise.
 **/
bool verifyExtensions();


/**
 * \func CreateDebugUtilsMessengerEXT
 *
 * This function calls to Vulkan extensions.
 * To do so vkGetInstanceProcAddr will acquire the corresponding function pointer,
 * or a nullptr if the extension cannot be loaded.
 **/
VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger);


/**
 * \func DestroyDebugUtilsMessengerEXT
 *
 * This function calls to Vulkan extensions.
 * To do so vkGetInstanceProcAddr will acquire the corresponding function pointer,
 * or a nullptr if the extension cannot be loaded.
 **/
void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator);


/**
 * \func readFile
 **/
static std::vector<char> readFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (not file.is_open())
    {
        throw std::runtime_error("Failed to open file.");
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}
