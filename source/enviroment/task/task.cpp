#include "task.hpp"


namespace blocks
{
  Task::Task(std::function<void()> function) : 
    isCanceled_(false),
    function_(function)
  {
  }

  void Task::Execute()
  {
    if (isCanceled_)
    {
      return;
    }

    function_();
  }
}
