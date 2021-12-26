#include "HelloTriangle.hpp"
#include "VulkanUtilities.hpp"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <set>
#include <stdexcept>
#include <iostream>
#include <vector>


#ifdef NDEBUG
  constexpr bool enableValidationLayers = false;
#else
  constexpr bool enableValidationLayers = true;
#endif // NDEBUG


void HelloTriangleApplication::initWindow()
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window_ = glfwCreateWindow(width_px_, height_px_, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::initVulkan()
{
  createInstance();
  setupDebugMessenger();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapChain();
  createImageViews();
}

void HelloTriangleApplication::mainLoop()
{
  while (!glfwWindowShouldClose(window_))
  {
    glfwPollEvents();
  }
}

void HelloTriangleApplication::cleanup()
{
  for (auto imageView : swapChainImageViews_)
  {
    vkDestroyImageView(device_, imageView, nullptr);
  }
  vkDestroySwapchainKHR(device_, swapchain_, nullptr);
  vkDestroyDevice(device_, nullptr);

  if (enableValidationLayers)
  {
    vkutil::destroyDebugUtilsMessengerEXT(instance_, debugMessenger_, nullptr);
  }

  vkDestroySurfaceKHR(instance_, surface_, nullptr);

  vkDestroyInstance(instance_, nullptr);

  glfwDestroyWindow(window_);

  glfwTerminate();
}

void HelloTriangleApplication::createInstance()
{
  if (enableValidationLayers && !checkValidationLayerSupport())
  {
    throw std::runtime_error("Validation layers requested, but not available");
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

  auto extensions = getRequiredExtensions();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  // This object must last for the duration of the vkCreateInstance call.
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (enableValidationLayers)
  {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers_.size());
    createInfo.ppEnabledLayerNames = validationLayers_.data();

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = &debugCreateInfo;
  }
  else
  {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create Vulkan instance");
  }
}

bool HelloTriangleApplication::checkValidationLayerSupport()
{
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const auto& validationLayerName : validationLayers_)
  {
    const auto match = std::find_if(std::cbegin(availableLayers), std::cend(availableLayers),
                 [&validationLayerName](const VkLayerProperties& availableLayer) {
                   return std::strcmp(availableLayer.layerName, validationLayerName) == 0;
                 });
    if (match == std::cend(availableLayers))
    {
      return false;
    }
  }

  return true;
}

std::vector<const char*> HelloTriangleApplication::getRequiredExtensions()
{
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;

  glfwExtensions =
    glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

  if (enableValidationLayers)
  {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApplication::debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData)
{
  spdlog::info("validation layer: {:s}", pCallbackData->pMessage);
  return VK_FALSE;
}

void HelloTriangleApplication::setupDebugMessenger()
{
  if (!enableValidationLayers)
  {
    return;
  }

  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  populateDebugMessengerCreateInfo(createInfo);

  if (vkutil::createDebugUtilsMessengerEXT(instance_, &createInfo, nullptr, &debugMessenger_) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to set up debug messenger");
  }
}

void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType =
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;
}

void HelloTriangleApplication::pickPhysicalDevice()
{
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);

  if (deviceCount == 0)
  {
    throw std::runtime_error("Failed to find GPUs with Vulkan support");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

  for (const auto& device : devices)
  {
    if (isDeviceSuitable(device))
    {
      physicalDevice_ = device;
      break;
    }
  }

  if (physicalDevice_ == VK_NULL_HANDLE)
  {
    throw std::runtime_error("Failed to find a suitable GPU");
  }
}

bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice device)
{
  const auto extensionsSupported = checkDeviceExtensionSupport(device);
  bool swapChainAdequate = false;
  if (extensionsSupported)
  {
    const auto swapChainSupport = querySwapChainSupport(device);
    swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
  }
  return findQueueFamilies(device).isComplete() && extensionsSupported && swapChainAdequate;
}

QueueFamilyIndices HelloTriangleApplication::findQueueFamilies(VkPhysicalDevice device)
{
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  uint32_t idx = 0;
  for (const auto& queueFamily : queueFamilies)
  {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphicsFamily = idx;
    }

    VkBool32 presentSupport{false};
    vkGetPhysicalDeviceSurfaceSupportKHR(device, idx, surface_, &presentSupport);
    if (presentSupport)
    {
      indices.presentFamily = idx;
    }

    if (indices.isComplete())
    {
      break;
    }
    idx++;
  }

  return indices;
}

void HelloTriangleApplication::createLogicalDevice()
{
  const auto indices = findQueueFamilies(physicalDevice_);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies = {*indices.graphicsFamily, *indices.presentFamily};

  float queuePriority = 1.0f;
  for (const auto& queueFamilyIdx : uniqueQueueFamilies)
  {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIdx;
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

  createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions_.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions_.data();

  if (enableValidationLayers)
  {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers_.size());
    createInfo.ppEnabledLayerNames = validationLayers_.data();
  }
  else
  {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create logical device");
  }

  vkGetDeviceQueue(device_, *indices.graphicsFamily, 0, &graphicsQueue_);
  vkGetDeviceQueue(device_, *indices.presentFamily, 0, &presentQueue_);
}

void HelloTriangleApplication::createSurface()
{
  if (glfwCreateWindowSurface(instance_, window_, nullptr, &surface_) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create window surface");
  }
}

bool HelloTriangleApplication::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

  std::set<std::string> requiredExtensions(std::begin(deviceExtensions_), std::end(deviceExtensions_));

  for (const auto& extension : availableExtensions)
  {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

SwapChainSupportDetails HelloTriangleApplication::querySwapChainSupport(VkPhysicalDevice device)
{
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, nullptr);

  if (formatCount != 0)
  {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, nullptr);

  if (presentModeCount != 0)
  {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, details.presentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR HelloTriangleApplication::chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
  for (const auto& availableFormat : availableFormats)
  {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
        && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR HelloTriangleApplication::chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes)
{
  for (const auto& availablePresentMode : availablePresentModes)
  {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
    {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D HelloTriangleApplication::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
  if (capabilities.currentExtent.width != UINT32_MAX)
  {
    return capabilities.currentExtent;
  }

  VkExtent2D actualExtent = {width_px_, height_px_};
  actualExtent.width = std::max(capabilities.minImageExtent.width,
      std::min(capabilities.maxImageExtent.width, actualExtent.width));
  actualExtent.height = std::max(capabilities.minImageExtent.height,
      std::min(capabilities.maxImageExtent.height, actualExtent.height));

  return actualExtent;
}

void HelloTriangleApplication::createSwapChain()
{
  const auto swapChainSupport = querySwapChainSupport(physicalDevice_);
  const auto surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
  const auto presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  const auto extent = chooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0
      && imageCount > swapChainSupport.capabilities.maxImageCount)
  {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface_;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  const auto indices = findQueueFamilies(physicalDevice_);
  std::array<uint32_t, 2> queueFamilyIndices = {*indices.graphicsFamily, *indices.presentFamily};

  if (indices.graphicsFamily != indices.presentFamily)
  {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
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

  if (vkCreateSwapchainKHR(device_, &createInfo, nullptr, &swapchain_) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create swap chain");
  }

  vkGetSwapchainImagesKHR(device_, swapchain_, &imageCount, nullptr);
  swapChainImages_.resize(imageCount);
  vkGetSwapchainImagesKHR(device_, swapchain_, &imageCount, swapChainImages_.data());
  swapChainImageFormat_ = surfaceFormat.format;
  swapChainExtent_ = extent;
}

void HelloTriangleApplication::createImageViews()
{
  swapChainImageViews_.resize(swapChainImages_.size());
  for (std::size_t idx = 0; idx < swapChainImages_.size(); ++idx)
  {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapChainImages_[idx];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainImageFormat_;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;
    if (vkCreateImageView(device_, &createInfo, nullptr, &swapChainImageViews_[idx]) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to create image views");
    }
  }
}
