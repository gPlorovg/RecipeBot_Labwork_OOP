#include <iostream>
#include "Ingredient.h"

Ingredient::Ingredient():name(" "), unit(" "), count(0) {};
Ingredient::Ingredient(const std::string &name, const std::string &unit,
                       int c):name(name), unit(unit) {
  if (c > 0)
    count = c;
  else
    std::cerr << "Error in 'Ingredient::Ingredient(const std::string &name, "
                 "const std::string &unit, int c)'\nCount must be "
                 "positive number\nReceived: " << c << std::endl;

};
Ingredient::Ingredient(const Ingredient &ingr) noexcept {
  name = ingr.GetName();
  unit = ingr.GetUnit();
  count = ingr.GetCount();
}
std::string Ingredient::Print() const {
  return "[" + std::to_string(count) + " " + unit + " of " + name + "]";
}
std::string Ingredient::GetName() const {
  return name;
}
std::string Ingredient::GetUnit() const {
  return unit;
}
int Ingredient::GetCount() const {
  return count;
}
void Ingredient::SetUnit(const std::string &u) {
  unit = u;
}
void Ingredient::SetCount(int c) {
  if (c > 0)
    count = c;
  else
    std::cerr << "Error in 'void Ingredient::SetCount(int c)'\nCount must be"
                 " positive number\nReceived: " << c << std::endl;
}