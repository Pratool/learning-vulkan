#include "HelloTriangle.hpp"


void HelloTriangleApplication::initWindow()
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window_ = glfwCreateWindow(width_px_, height_px_, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::initVulkan()
{
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
  glfwDestroyWindow(window_);

  glfwTerminate();
}
