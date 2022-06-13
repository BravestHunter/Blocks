#pragma once

#include "export.h"
#include "resource/resource_base.hpp"
#include "platform/glfw_platform.hpp"


namespace blocks
{
  class DllExport Environment
  {
  private:
    Environment() = delete;

  public:
    static void Init();
    static void Deinit();

    static GlfwPlatform& GetPlatform();
    static ResourceBase& GetResource();

  private:
    static GlfwPlatform platform_;
    static ResourceBase resource_;
  };
}
