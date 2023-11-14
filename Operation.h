#ifndef LAB1__OPERATION_H_
#define LAB1__OPERATION_H_

#include <string>

class Operation final{
 public:
  Operation();
  Operation(const std::string &action, float time);
  Operation(const Operation &op) noexcept ;
  [[nodiscard]] std::string GetAction() const;
  [[nodiscard]] float GetTime() const;
  [[nodiscard]] float GetTime(int count) const;
  std::string Print(int count=1) const;
  void SetTime(float t);
 private:
  std::string action;
  float time;
};

#endif //LAB1__OPERATION_H_
