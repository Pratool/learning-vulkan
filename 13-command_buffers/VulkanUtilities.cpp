#include "VulkanUtilities.hpp"

#include <fstream>
#include <iostream>

namespace vkutil
{

std::vector<char> readFile(const std::string& filename)
{
  std::ifstream ifs(filename, std::ios::ate | std::ios::binary);
  if (!ifs)
  {
    throw std::runtime_error("Failed to read file");
  }

  const auto filesize = ifs.tellg();
  std::vector<char> buffer(filesize);
  ifs.seekg(0);
  ifs.read(buffer.data(), filesize);
  ifs.close();

  return buffer;
}

VkResult createDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
  auto fn = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
    vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

  if (fn != nullptr)
  {
    return fn(instance, pCreateInfo, pAllocator, pDebugMessenger);
  }

  return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator)
{
  auto fn = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
    vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

  if (fn != nullptr)
  {
    fn(instance, debugMessenger, pAllocator);
  }
}

VkShaderModule createShaderModuleFromSpvBytecode(const VkDevice& device, const std::string& shaderSourceFile)
{
  const auto code = readFile(shaderSourceFile);
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
      throw std::runtime_error("failed to create shader module!");
  }

  return shaderModule;
}

} // namespace vkutil
