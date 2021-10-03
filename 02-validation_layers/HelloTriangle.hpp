#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <vector>


class HelloTriangleApplication
{
private:
  GLFWwindow* window_;
  const uint32_t width_px_ = 800;
  const uint32_t height_px_ = 600;

  const std::array<const char*, 1> validationLayers_ = {
    "VK_LAYER_KHRONOS_validation"
  };

  /// Connects the app to the Vulkan library.
  VkInstance instance_;

  /// Handles debug logging.
  VkDebugUtilsMessengerEXT debugMessenger_;

public:
  inline void run();

private:
  void initWindow();

  /// Top-level initalization function to set up Vulkan objects.
  void initVulkan();
  /// Initialize an instance to connect this app to the Vulkan library.
  /// Specify details about the app to the driver.
  void createInstance();
  /// Adds runtime error checking of Vulkan API calls.
  bool checkValidationLayerSupport();
  /// Get the required list of extensions based on whether validation layers are enabled.
  std::vector<const char*> getRequiredExtensions();
  /// Configure debug logging with Vulkan.
  void setupDebugMessenger();
  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

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
