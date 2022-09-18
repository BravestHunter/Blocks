#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "task.hpp"


namespace blocks
{
  class TaskScheduler
  {
  private:
    const int ThreadsNumber = 4;

  public:
    TaskScheduler();
    TaskScheduler(const TaskScheduler&) = delete;
    TaskScheduler(TaskScheduler&& other) = delete;
    TaskScheduler& operator=(const TaskScheduler&) = delete;
    TaskScheduler& operator=(TaskScheduler&& other) = delete;
    ~TaskScheduler();

    void Start();
    void Stop();

    void EnqueueTask(std::shared_ptr<Task> task);

  private:
    void ProcessTasks();

    std::vector<std::thread> threads_;
    std::queue<std::shared_ptr<Task>> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stopRequested_ = false ;
  };
}
