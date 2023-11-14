#ifndef LAB1__RECIPE_H_
#define LAB1__RECIPE_H_

#include <string>
#include "Queue.h"
#include "Tree.h"

class Recipe {
 public:
  Recipe();
  Recipe(const std::string& name_, float time_, Queue* data_);
  Recipe(const Recipe &rc);
  ~Recipe();

  [[nodiscard]] std::string GetName() const;
  [[nodiscard]] float GetTime() const;
  virtual std::string Print() const;

 protected:
  std::string name;
  float time;
 private:
  Queue* data;
};

class RecipeTree final: public Recipe {
 public:
  RecipeTree();
  RecipeTree(const std::string& name_, float time_, Tree* data_);
  RecipeTree(const RecipeTree &rt);
  ~RecipeTree();

  std::string Print() const override;
 private:
  Tree* data;
};

#endif //LAB1__RECIPE_H_
