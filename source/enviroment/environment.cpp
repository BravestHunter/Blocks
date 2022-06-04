#include "environment.hpp"


namespace blocks
{
  void Enviroment::InitSystems()
  {
    platform_.Init();
    render_.Init();
    resource_.Init();
  }

  void Enviroment::DeinitSystems()
  {
    resource_.Deinit();
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

  ResourceBase& Enviroment::GetResourceSystem()
  {
    return resource_;
  }


  GlfwPlatform Enviroment::platform_ = GlfwPlatform();
  OpenglRenderSystem Enviroment::render_ = OpenglRenderSystem();
  ResourceBase Enviroment::resource_ = ResourceBase();
}
