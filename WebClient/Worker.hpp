#ifndef WORKER_HPP
#define WORKER_HPP

#include <thread>
#include <queue>
#include <atomic>
#include <functional>

class Worker
{
private:
  std::thread thread_;
  std::queue<std::function<void()>> queue_;
  std::atomic<bool> isRunning_;
  std::mutex mutex_;
  void run();

public:
  Worker();

  ~Worker();

  void addTask(const std::function<void()>& function);

  bool isWaiting();

};


#endif
