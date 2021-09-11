#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class HelloTriangleApplication
{
private:
  GLFWwindow* window_;
  const uint32_t width_px_ = 800;
  const uint32_t height_px_ = 600;

public:
  inline void run();

private:
  void initWindow();

  /// Top-level initalization function to set up Vulkan objects.
  void initVulkan();

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
