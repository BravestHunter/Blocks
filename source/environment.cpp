#include "environment.hpp"


void Enviroment::InitSystems()
{
  platform_.Init();
  render_.Init();
}

void Enviroment::DeinitSystems()
{
  render_.Deinit();
  platform_.Deinit();
}


GlfwPlatform& Enviroment::GetPlatformSystem()
{
  return platform_;
}

OpenglRenderSystem& Enviroment::GetRenderSystem()
{
  return render_;
}


GlfwPlatform Enviroment::platform_ = GlfwPlatform();
OpenglRenderSystem Enviroment::render_ = OpenglRenderSystem();
