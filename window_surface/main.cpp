#include "HelloTriangle.hpp"

#include <spdlog/spdlog.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
  HelloTriangleApplication app;

  try
  {
    app.run();
  }
  catch (const std::exception& e)
  {
    spdlog::error(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
