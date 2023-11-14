#include <iostream>
#include "Recipe.h"

Recipe::Recipe(): name(" "), time(0), data(nullptr) {};
Recipe::Recipe(const std::string &name_, float time_, Queue* data_) {
  name = name_;
  data = data_;
  if (time_ > 0)
    time = time_;
  else
    std::cout << "Error in Recipe::Recipe(const std::string name_, float time_,"
                 " Node<Ingredient> *dataHead_)\n Time can't be " << time_
              << std::endl;
}
Recipe::Recipe(const Recipe &rc): name(rc.name), time(rc.time), data(rc.data)
{};
Recipe::~Recipe() {
  delete data;
  data = nullptr;
}
std::string Recipe::GetName() const {
  return name;
}
float Recipe::GetTime() const {
  return time;
}
std::string Recipe::Print() const {
  std::stringstream stream;
  stream << name << " | " << time << '\n';
  QueueNode* curr;
  curr = data->GetHead();
  int currIngrCount = 1;
  while (curr != nullptr) {
    if (curr->type == NodeType::Ingredient) {
      stream << static_cast<Ingredient*>(curr->data)->Print();
      currIngrCount = static_cast<Ingredient*>(curr->data)->GetCount();
    } else if (curr->type == NodeType::Operation){
      stream << static_cast<Operation*>(curr->data)->Print(currIngrCount);
    }
    stream <<'\n';
    curr = curr->next;
  }
  return stream.str();
}

RecipeTree::RecipeTree(): Recipe() {};
RecipeTree::RecipeTree(const std::string& name_, float time_, Tree* data_):
    Recipe(name_, time_, nullptr), data(data_) {};
RecipeTree::RecipeTree(const RecipeTree &rt):
    Recipe(rt.name, rt.time, nullptr), data(rt.data){};
RecipeTree::~RecipeTree() {
  delete data;
  data = nullptr;
};
std::string RecipeTree::Print() const {
  std::stringstream stream;
  stream << name << " | " << time << '\n';
  return stream.str() + data->Print();
}
