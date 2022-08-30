#pragma once

#include <queue>
#include <mutex>


namespace blocks
{
  // Simpliest implementation
  template <typename T>
  class ConcurrentQueue
  {
  public:
    void Push(T item);
    T Pop();
    inline bool IsEmpty();

  private:
    std::queue<T> queue_;
    std::mutex mutex_;
  };


  template <typename T>
  void ConcurrentQueue<T>::Push(T item)
  {
    std::lock_guard lock(mutex_);

    queue_.push(item);
  }

  template <typename T>
  T ConcurrentQueue<T>::Pop()
  {
    std::lock_guard lock(mutex_);

    T item = queue_.front();
    queue_.pop();

    return item;
  }

  template <typename T>
  bool ConcurrentQueue<T>::IsEmpty()
  {
    return queue_.empty();
  }
}
