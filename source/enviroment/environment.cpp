#include "environment.hpp"


namespace blocks
{
  void Environment::Init()
  {
    taskScheduler_.Start();
    platform_.Init();
  }

  void Environment::Deinit()
  {
    taskScheduler_.Stop();
    platform_.Deinit();
  }


  TaskScheduler& Environment::GetTaskScheduler()
  {
    return taskScheduler_;
  }

  GlfwPlatform& Environment::GetPlatform()
  {
    return platform_;
  }

  ResourceBase& Environment::GetResource()
  {
    return resource_;
  }


  TaskScheduler Environment::taskScheduler_ = TaskScheduler();
  GlfwPlatform Environment::platform_ = GlfwPlatform();
  ResourceBase Environment::resource_ = ResourceBase();
}
