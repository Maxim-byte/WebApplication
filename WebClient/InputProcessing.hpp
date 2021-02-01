#ifndef INPUTPROCESSING_HPP
#define INPUTPROCESSING_HPP

#include <string>
#include "SingleThreadPool.hpp"

static std::exception_ptr ptr = nullptr;
std::exception_ptr & getPtr();

class InputProcessing
{
public:
  static void validateInputString(std::string & input);

  static void sortAndTransformInputString(std::string & inputValidateString);

  static long printInfo(std::string & modifiedInput);

  static long countOfNum(std:: string & modifiedInput);

  static std::string firstStageProcessingInput(std::string inputString);

  static long secondStageProcessingInput(std::shared_ptr<SingleThreadPool::future_t<std::string>>  modifiedInput);

};


#endif
