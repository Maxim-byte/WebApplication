#ifndef INVALIDINPUTFORMAT_HPP
#define INVALIDINPUTFORMAT_HPP

#include <exception>
#include <string>

class InvalidInputFormat : public std::exception
{
private:
  std::string error_;
public:
  explicit InvalidInputFormat(const char *msg);

  const char* what() const noexcept override;
};


#endif
