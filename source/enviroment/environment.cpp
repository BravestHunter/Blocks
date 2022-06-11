#include "environment.hpp"


namespace blocks
{
  void Enviroment::InitSystems()
  {
    platform_.Init();
    resource_.Init();
  }

  void Enviroment::DeinitSystems()
  {
    resource_.Deinit();
    platform_.Deinit();
  }


  GlfwPlatform& Enviroment::GetPlatformSystem()
  {
    return platform_;
  }

  ResourceBase& Enviroment::GetResourceSystem()
  {
    return resource_;
  }


  GlfwPlatform Enviroment::platform_ = GlfwPlatform();
  ResourceBase Enviroment::resource_ = ResourceBase();
}
