#pragma once

#include "export.h"
#include "platform/glfw_platform.hpp"


class DllExport Enviroment
{
private:
  Enviroment() = delete;

public:
  static void InitSystems();
  static void DeinitSystems();
  
  static GlfwPlatform& GetPlatformSystem();

private:
  static GlfwPlatform platform_;
};
