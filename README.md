# Learning Vulkan
Version controlled source code while learning about Vulkan.
Resources:
- [VulkanTutorial](https://vulkan-tutorial.com):
  Can find the git source [on GitHub](https://github.com/Overv/VulkanTutorial).

## Development environment
Mostly taken from [VulkanTutorial's docs](https://vulkan-tutorial.com/en/Development_environment).
1. Download the appropriate SDK [from LunarG's website](https://vulkan.lunarg.com/sdk/home).
2. [Linux only] Source `setup-env.sh` to configure environment variables.
3. Build and install external projects as outlined in `./external/README.md`.
4. [Linux only] Configure `LD_LIBRARY_PATH` to include the library installation directory from step 3.
   This is most likely `CMAKE_INSTALL_PREFIX/lib`.
   Also include the source installation directory after running the build script in this directory.
