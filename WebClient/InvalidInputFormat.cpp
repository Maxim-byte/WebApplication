#include <cstring>
#include "InvalidInputFormat.hpp"

InvalidInputFormat::InvalidInputFormat(const char *msg):
  error_(msg)
{}

const char *InvalidInputFormat::what() const noexcept
{
  return error_.c_str();
}
