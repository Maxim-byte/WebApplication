#ifndef GLOBALVAR_HPP
#define GLOBALVAR_HPP

#include <exception>
#include <atomic>
#include "SingleThreadPool.hpp"

namespace clientVar
{
  static const int BUFFER_SIZE = 256;
  static const size_t MAX_INPUT_STRING_LENGTH = 64;
  static SingleThreadPool FirstThreadPool;
  static SingleThreadPool SecondThreadPool;
}

#endif
