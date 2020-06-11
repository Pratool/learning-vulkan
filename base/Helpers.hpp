#pragma once

// external library headers
#include <vulkan/vulkan_core.h>


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
