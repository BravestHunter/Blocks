#include "time_module.hpp"

#include "environment.hpp"


namespace blocks
{
  TimeModule::TimeModule() : state_()
  {
    Init();
  }


  void TimeModule::Update()
  {
    GlfwPlatform& platform = Environment::GetPlatform();

    state_.currentTime_ = Environment::GetPlatform().GetTime();
  }

  void TimeModule::OnSceneChanged()
  {
    Init();
  }


  void TimeModule::Init()
  {
    state_.initTime_ = Environment::GetPlatform().GetTime();
  }
}
