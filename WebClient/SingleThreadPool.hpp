#ifndef FIXEDTHREADPOOL_HPP
#define FIXEDTHREADPOOL_HPP

#include <thread>
#include <memory>
#include <functional>
#include <future>
#include "Worker.hpp"

class SingleThreadPool
{
public:
  template<class T>
  struct future_t
  {
    future_t():
      isReady(false)
    {}
    T data;
    bool isReady;
  };
private:
  std::shared_ptr<Worker> worker_;

public:
  SingleThreadPool();

  ~SingleThreadPool() = default;

  template<class Function, class... Args>
  void execute(Function func, Args... arguments)
  {
    worker_->addTask(static_cast<std::function<void()> &&>(std::bind(func, arguments...)));
  }

  template<class ReturnValue, class Function, class... Args>
  std::shared_ptr<future_t<ReturnValue>> execute(Function func, Args... arguments)
  {
    auto function = std::bind(func, arguments...);
    std::shared_ptr<future_t<ReturnValue>> future(new future_t<ReturnValue>);
    std::function<void()> wrapper = [=]()
    {
      future->data = function();
      future->isReady = true;
    };
    worker_->addTask(wrapper);
    return future;
  }

  bool isWaiting();
};

#endif
