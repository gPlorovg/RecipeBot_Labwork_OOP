#include <iostream>
#include "Operation.h"

Operation::Operation():action(" "), time(0) {};
Operation::Operation(const std::string &action, float t):action(action) {
  if (t > 0)
    time = t;
  else
    std::cerr << "Error in 'Operation::Operation(const std::string &action, "
                 "float t)'\nTime must be positive number.\nReceived: " << t
              << std::endl;
};
Operation::Operation(const Operation &op) noexcept {
  action = op.GetAction();
  time = op.GetTime();
}
std::string Operation::Print(int count) const {
  return "(" + action + " : " + std::to_string(GetTime(count)) + ")";
}
std::string Operation::GetAction() const {
  return action;
}
float Operation::GetTime() const {
  return time;
}
float Operation::GetTime(int count) const {
  if (count > 0)
    return time * (float)count;
  else
    std::cerr << "Error in 'float Operation::GetTime(int count) const'\n"
                 "Count must be positive number\nReceived: " << count <<
              std::endl;
  return time;
}
void Operation::SetTime(float t) {
  if (t > 0) {
    time = t;
  } else {
    std::cerr << "Error in 'void Operation::SetTime(float t)'\nTime must be "
                 "positive number.\nReceived: " << t << std::endl;
  }
}