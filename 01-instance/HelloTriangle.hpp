#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class HelloTriangleApplication
{
private:
  GLFWwindow* window_;
  const uint32_t width_px_ = 800;
  const uint32_t height_px_ = 600;

  /// Connects the app to the Vulkan library.
  VkInstance instance_;

public:
  inline void run();

private:
  void initWindow();

  /// Top-level initalization function to set up Vulkan objects.
  void initVulkan();
  /// Initialize an instance to connect this app to the Vulkan library.
  /// Specify details about the app to the driver.
  void createInstance();

  void mainLoop();

  /// Manually free resources after the program is intended to terminate.
  void cleanup();
};


void HelloTriangleApplication::run()
{
  initWindow();
  initVulkan();
  mainLoop();
  cleanup();
}
