#include "InputProcessing.hpp"
#include <cctype>
#include <iostream>
#include <memory>
#include <cmath>
#include <algorithm>
#include "GlobalVar.hpp"
#include "InvalidInputFormat.hpp"

std::exception_ptr &getPtr()
{
  return ptr;
}

void InputProcessing::validateInputString(std::string &input)
{
  size_t countOfChar = 0;
  for (const auto &ch : input)
  {
    ++countOfChar;
    if (!isdigit(ch) || countOfChar > clientVar::MAX_INPUT_STRING_LENGTH)
    {
      throw InvalidInputFormat("Incorrect format input string");
    }
  }
}

void InputProcessing::sortAndTransformInputString(std::string &inputValidateString)
{
  std::sort(inputValidateString.begin(), inputValidateString.end(), [](const auto lhs, const auto rhs)
  {
    return rhs < lhs;
  });
  std::string temp;
  for (char i : inputValidateString)
  {
    if (i % 2 == 0)
    {
      temp.push_back('K');
      temp.push_back('B');
      continue;
    }
    temp.push_back(i);
  }
  inputValidateString = std::move(temp);
}

long InputProcessing::countOfNum(std::string &modifiedInput)
{
  long sum = 0;
  long current = 0;
  size_t pow = 1;
  for(int i = modifiedInput.size(); i >= 0; --i)
  {
    if(isdigit(modifiedInput[i]))
    {
      current += (modifiedInput[i] - '0') * pow;
      pow *= 10;
    }
    else
    {
      sum += current;
      pow = 1;
      current = 0;
    }
  }
  sum += current;
  return sum;
}

long InputProcessing::printInfo(std::string &modifiedInput)
{
  std::cout << "Modified input: " << modifiedInput << '\n';
  long temp = countOfNum(modifiedInput);
  std::cout << "Count of num: " << temp << "\n";
  std::cout.flush();
  return temp;
}

std::string InputProcessing::firstStageProcessingInput(std::string inputString)
{
  try
  {
    validateInputString(inputString);
    sortAndTransformInputString(inputString);
  }
  catch (...)
  {
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    ptr = std::current_exception();
  }
  return inputString;
}

long InputProcessing::secondStageProcessingInput(std::shared_ptr<SingleThreadPool::future_t<std::string>> modifiedInput)
{
  while (!modifiedInput->isReady)
  {
    //waiting
  }
  try
  {
    return printInfo(modifiedInput->data);
  }
  catch (...)
  {
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    ptr = std::current_exception();
  }
  return -1;
}
