#include <vulkan/vulkan.h>

#include <string>
#include <vector>

namespace vkutil
{

/// Reads a file into memory as a vector of chars.
/// @note Throws an exception if std::ifstream::operator bool() returns false.
std::vector<char> readFile(const std::string& filename);

VkResult createDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator);

/// Creates a Vulkan shader module from the compiled SPIR-V bytecode file.
VkShaderModule createShaderModuleFromSpvBytecode(const VkDevice& device, const std::string& shaderSourceFIle);

} // namespace vkutil
