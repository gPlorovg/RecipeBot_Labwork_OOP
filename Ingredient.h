#ifndef LAB1__INGREDIENT_H_
#define LAB1__INGREDIENT_H_

#include <string>

class Ingredient final{
 public:
  Ingredient();
  Ingredient(const std::string &name, const std::string &unit, int c);
  Ingredient(const Ingredient &ingr) noexcept ;
  [[nodiscard]] int GetCount() const;
  [[nodiscard]] std::string GetName() const;
  [[nodiscard]] std::string GetUnit() const;
  std::string Print() const;
  void SetCount(int c);
  void SetUnit(const std::string &u);
 private:
  std::string name;
  std::string unit;
  int count;
};

#endif //LAB1__INGREDIENT_H_
