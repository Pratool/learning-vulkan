#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <vector>
#include <optional>


struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  inline bool isComplete() const noexcept
  {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};


struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};


class HelloTriangleApplication
{
private:
  GLFWwindow* window_;
  const uint32_t width_px_ = 800;
  const uint32_t height_px_ = 600;

  const std::array<const char*, 1> validationLayers_ = {
    "VK_LAYER_KHRONOS_validation"
  };

  const std::array<const char*, 1> deviceExtensions_ = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

  /// Connects the app to the Vulkan library.
  VkInstance instance_;

  /// Handles debug logging.
  VkDebugUtilsMessengerEXT debugMessenger_;

  /// Surface used to display graphics output.
  VkSurfaceKHR surface_;

  /// Graphics card device handle.
  VkPhysicalDevice physicalDevice_{VK_NULL_HANDLE};

  /// Logical device handle.
  VkDevice device_;

  /// Queue handles associated with the logical device.
  VkQueue graphicsQueue_;
  VkQueue presentQueue_;

  /// Hold the swapchain object.
  VkSwapchainKHR swapchain_;
  std::vector<VkImage> swapChainImages_;
  VkFormat swapChainImageFormat_;
  VkExtent2D swapChainExtent_;

  std::vector<VkImageView> swapChainImageViews_;

  VkRenderPass renderPass_;
  VkPipelineLayout pipelineLayout_;

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
  /// Creates the surface to display graphics output.
  void createSurface();
  /// Picks the physical device with a Vulkan API for use in the app.
  void pickPhysicalDevice();
  /// Determine if physical device is usable for the app.
  bool isDeviceSuitable(VkPhysicalDevice device);
  /// Assigns indices to queue families that could be found.
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  /// Creates a handle to access selected physical device.
  void createLogicalDevice();
  /// Check that extensions needed for the app exist for this device.
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
  void createSwapChain();
  void createImageViews();
  void createRenderPass();
  void createGraphicsPipeline();

  template <typename Container>
  inline VkShaderModule createShaderModule(const Container& code);

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

template <typename Container>
VkShaderModule HelloTriangleApplication::createShaderModule(const Container& code)
{
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size()*4;
  createInfo.pCode = code.data();

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(device_, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create shader module");
  }

  return shaderModule;
}
