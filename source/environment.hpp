#pragma once

#include "export.h"
#include "platform/glfw_platform.hpp"
#include "render/opengl_render_system.hpp"


class DllExport Enviroment
{
private:
  Enviroment() = delete;

public:
  static void InitSystems();
  static void DeinitSystems();
  
  static GlfwPlatform& GetPlatformSystem();
  static OpenglRenderSystem& GetRenderSystem();

private:
  static GlfwPlatform platform_;
  static OpenglRenderSystem render_;
};
