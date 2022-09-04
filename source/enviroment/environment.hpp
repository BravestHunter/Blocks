#pragma once

#include "export.h"
#include "task/task_scheduler.hpp"
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

    static TaskScheduler& GetTaskScheduler();
    static GlfwPlatform& GetPlatform();
    static ResourceBase& GetResource();

  private:
    static TaskScheduler taskScheduler_;
    static GlfwPlatform platform_;
    static ResourceBase resource_;
  };
}
