# Learning Vulkan
Version controlled source code while learning about Vulkan.
Resources:
- [VulkanTutorial](https://vulkan-tutorial.com):
  Can find the git source [on GitHub](https://github.com/Overv/VulkanTutorial).

## Development environment
This is ultimately up for the developer to decide.
In order to replicate running build scripts in this repository, follow this structure.
Note: This development environment setup process is intended to be run on Linux.

Create the following directories adjacent to directory containing this repository.
Here, the repository is in the directory `learning-vulkan`.
.
├── app-build
├── app-install
├── build
├── install
├── learning-vulkan
└── vulkan-sdk

1. Build and install external projects as outlined in `./external/README.md`.
   Note: RPATHs will be set in the ELF executable to point to this installation.
   Note: RPATHs are not supported on all operating systems.

2. Download and extract the Vulkan SDK into `vulkan-sdk`.
   The SDK can be found on [LunarG's website](https://vulkan.lunarg.com/sdk/home).

3. Source the `setup-env.sh` file in the Vulkan SDK to configure environment variables.

Consider installing `direnv` and write `source vulkan-sdk/1.2.189.0/setup-env.sh` into the `.envrc` file.
The `.envrc` file should be located adjacent to the `vulkan-sdk` directory.
