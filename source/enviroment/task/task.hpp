#pragma once

#include <functional>


namespace blocks
{
  class Task
  {
    public:
      Task(std::function<void()> function);

      void Execute();
      inline void Cancel();

      inline bool IsCanceled() const;

  private:
    std::function<void()> function_;
    bool isCanceled_;
  };


  void Task::Cancel()
  {
    isCanceled_ = true;
  }


  bool Task::IsCanceled() const
  {
    return isCanceled_;
  }
}
