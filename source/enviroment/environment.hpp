#pragma once

#include "export.h"
#include "resource/resource_base.hpp"
#include "platform/glfw_platform.hpp"
#include "render/opengl_render_system.hpp"


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
    static OpenglRenderSystem& GetRenderSystem();
    static ResourceBase& GetResourceSystem();

  private:
    static GlfwPlatform platform_;
    static OpenglRenderSystem render_;
    static ResourceBase resource_;
  };
}
