#include "HelloTriangle.hpp"

#include <stdexcept>


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
  vkDestroyInstance(instance_, nullptr);

  glfwDestroyWindow(window_);

  glfwTerminate();
}

void HelloTriangleApplication::createInstance()
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

  glfwExtensions =
    glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0;

  if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create Vulkan instance");
  }
}
