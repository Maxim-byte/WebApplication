#include "SingleThreadPool.hpp"

SingleThreadPool::SingleThreadPool():
  worker_(std::make_shared<Worker>())
{}

bool SingleThreadPool::isWaiting()
{
  return worker_->isWaiting();
}


