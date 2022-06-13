#include "environment.hpp"


namespace blocks
{
  void Environment::Init()
  {
    platform_.Init();
  }

  void Environment::Deinit()
  {
    platform_.Deinit();
  }


  GlfwPlatform& Environment::GetPlatform()
  {
    return platform_;
  }

  ResourceBase& Environment::GetResource()
  {
    return resource_;
  }


  GlfwPlatform Environment::platform_ = GlfwPlatform();
  ResourceBase Environment::resource_ = ResourceBase();
}
