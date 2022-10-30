#pragma once

#include "time_state.hpp"


namespace blocks
{
  class TimeModule
  {
  public:
    TimeModule();

    void Update();
    void OnSceneChanged();

    inline const TimeState& GetState() const;

  private:
    TimeState state_;

    void Init();
  };


  const TimeState& TimeModule::GetState() const
  {
    return state_;
  }
}
