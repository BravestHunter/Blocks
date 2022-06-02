#include "environment.hpp"


void Enviroment::InitSystems()
{
  platform_.Init();
}

void Enviroment::DeinitSystems()
{
  platform_.Deinit();
}


GlfwPlatform& Enviroment::GetPlatformSystem()
{
  return platform_;
}


GlfwPlatform Enviroment::platform_ = GlfwPlatform();
