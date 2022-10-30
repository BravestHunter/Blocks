#pragma once


namespace blocks
{
  class TimeModule;

  class TimeState
  {
    friend class TimeModule;

  public:
    inline double GetSceneTime() const;

  private:
    double initTime_;
    double currentTime_;
  };


  double TimeState::GetSceneTime() const
  {
    return currentTime_ - initTime_;
  }
}
