#include "environment.hpp"


namespace blocks
{
  void Enviroment::Init()
  {
    platform_.Init();
  }

  void Enviroment::Deinit()
  {
    platform_.Deinit();
  }


  GlfwPlatform& Enviroment::GetPlatform()
  {
    return platform_;
  }

  ResourceBase& Enviroment::GetResource()
  {
    return resource_;
  }


  GlfwPlatform Enviroment::platform_ = GlfwPlatform();
  ResourceBase Enviroment::resource_ = ResourceBase();
}
