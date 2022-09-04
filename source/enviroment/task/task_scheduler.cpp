#include "task_scheduler.hpp"


namespace blocks
{
  TaskScheduler::TaskScheduler()
  {
  }

  TaskScheduler::~TaskScheduler()
  {
  }


  void TaskScheduler::Start()
  {
    threads_.reserve(ThreadsNumber);
    for (size_t i = 0; i < ThreadsNumber; ++i) 
    {
      threads_.emplace_back(&TaskScheduler::ProcessTasks, this);
    }
  }

  void TaskScheduler::Stop()
  {
    {
      std::lock_guard<std::mutex> guard(mutex_);

      stopRequested_ = true;
    }

    cv_.notify_all();

    for (auto& thread : threads_) 
    {
      if (thread.joinable()) 
      {
        thread.join();
      }
    }
  }


  void TaskScheduler::EnqueueTask(Task task)
  {
    {
      std::lock_guard<std::mutex> guard(mutex_);

      queue_.push(std::move(task));
    }

    cv_.notify_one();
  }


  void TaskScheduler::ProcessTasks()
  {
    bool stopped = false;

    while (!stopped) {
      std::unique_lock<std::mutex> lock(mutex_);

      cv_.wait(lock, [this, &stopped]()
        {
          stopped = stopRequested_;
          return !queue_.empty() || stopped;
        }
      );

      if (stopped)
      {
        return;
      }

      Task task = queue_.front();
      queue_.pop();

      lock.unlock();

      task();
    }
  }
}
