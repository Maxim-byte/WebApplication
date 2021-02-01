#include "Worker.hpp"

Worker::Worker() :
        isRunning_(true),
        queue_(),
        thread_(&Worker::run, this)
{}

Worker::~Worker()
{
  isRunning_ = false;
  thread_.join();
}

void Worker::addTask(const std::function<void()> &function)
{
  std::lock_guard<std::mutex> lock(mutex_);
  queue_.push(function);
}

void Worker::run()
{
  while (isRunning_)
  {
    if (!queue_.empty())
    {
      auto function = queue_.front();
      function();
      std::unique_lock<std::mutex> lock(mutex_);
      queue_.pop();
      lock.unlock();
    } else
    {
      std::this_thread::yield();
    }
  }
}

bool Worker::isWaiting()
{
  return queue_.empty();
}
