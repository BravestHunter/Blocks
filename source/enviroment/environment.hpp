#pragma once

#include "export.h"
#include "resource/resource_base.hpp"
#include "platform/glfw_platform.hpp"


namespace blocks
{
  class DllExport Enviroment
  {
  private:
    Enviroment() = delete;

  public:
    static void InitSystems();
    static void DeinitSystems();

    static GlfwPlatform& GetPlatformSystem();
    static ResourceBase& GetResourceSystem();

  private:
    static GlfwPlatform platform_;
    static ResourceBase resource_;
  };
}
