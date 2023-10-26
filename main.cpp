#include <string>
#include <assert.h>
#include <iostream>

class Ingredient final{
 public:
  Ingredient();
  Ingredient(const std::string &name, const std::string &unit, int c);
  Ingredient(const Ingredient &ingr) noexcept ;
  [[nodiscard]] int GetCount() const;
  [[nodiscard]] std::string GetName() const;
  [[nodiscard]] std::string GetUnit() const;
  void SetCount(int c);
  void SetUnit(const std::string &u);
 private:
  std::string name;
  std::string unit;
  int count;
};

class Operation final{
 public:
  Operation();
  Operation(const std::string &action, float time);
  Operation(const Operation &op) noexcept ;
  [[nodiscard]] std::string GetAction() const;
  [[nodiscard]] float GetTime() const;
  [[nodiscard]] float GetTime(int count) const;
  void SetTime(float t);
 private:
  std::string action;
  float time;
};

enum class NodeType {None, Ingredient, Operation};

template <typename T>
class Node {
 public:
  Node();
  Node(NodeType type_, T* data_);
  Node(const Node &n);
  ~Node();

  [[nodiscard]] T* GetData() const;
  [[nodiscard]] NodeType GetType() const;
  [[nodiscard]] Node* GetNext() const;
  void SetNext(Node* child);

 protected:
  NodeType type;
  T* data;
 private:
  Node* next;
};


template <typename T>
class TreeNode final: public Node<T> {
 public:
  TreeNode();
  TreeNode(NodeType type_, T* data_);
  TreeNode(const TreeNode &tn);
  ~TreeNode();

  [[nodiscard]] int GetChildCount() const;
  [[nodiscard]] TreeNode* GetChild(int count) const;
  void AppendChild(TreeNode* node);

 private:
  int childCount;
  TreeNode** childList;
};


class Recipe {
 public:
  Recipe();
  Recipe(const std::string& name_, float time_, Node<Ingredient> *dataHead_);
  Recipe(const Recipe &rc);
  ~Recipe();

  std::string GetName() const;
  float GetTime() const;
  virtual void Print() const;

 protected:
  std::string name;
  float time;
 private:
  Node<Ingredient> *dataHead;
};

class RecipeTree final: public Recipe {
 public:
  void Print() const override;
 private:
  TreeNode<Ingredient> *dataHead;
};

Recipe::Recipe(): name(" "), time(0), dataHead(nullptr) {};
Recipe::Recipe(const std::string &name_, float time_, Node<Ingredient>
    *dataHead_) {
  name = name_;
  dataHead = dataHead_;
  if (time_ > 0)
    time = time_;
  else
    std::cout << "Error in Recipe::Recipe(const std::string name_, float time_,"
                 " Node<Ingredient> *dataHead_)\n Time can't be " << time_
                 << std::endl;
}
Recipe::Recipe(const Recipe &rc): name(rc.name), time(rc.time),
  dataHead(rc.dataHead) {};
Recipe::~Recipe() {
  delete dataHead;
}
std::string Recipe::GetName() const {
  return name;
}
float Recipe::GetTime() const {
  return time;
}
void Recipe::Print() const {
  Node<Ingredient> *p = dataHead;
  std::string outString;
  while (p != nullptr) {
    if (p->GetType() == NodeType::Ingredient) {
      outString = "[" + std::to_string(p->GetData()->GetCount()) + " " +
          p->GetData()->GetUnit() + " of " + p->GetData()->GetName() + "]";

    } else if (p->GetType() == NodeType::Operation){
      outString = "(" + p->GetData()->GetName() + " : " + p->GetData()->GetT
    }
  }
}

template <typename T>
Node<T>::Node(): type(NodeType::None), data(nullptr), next(nullptr) {};

template <typename T>
Node<T>::Node(NodeType type_, T* data_): type(type_), data(data_), next(nullptr)
{};

template <typename T>
Node<T>::Node(const Node &n): type(n.type), data(n.data), next(n.next) {};

template <typename T>
Node<T>::~Node() {
  delete data;
  delete next;
}

template <typename T>
T* Node<T>::GetData() const {
  return data;
}

template <typename T>
NodeType Node<T>::GetType() const {
  return type;
}

template <typename T>
Node<T>* Node<T>::GetNext() const {
  return next;
}

template <typename T>
void Node<T>::SetNext(Node<T> *child) {
  next = child;
}

template <typename T>
TreeNode<T>::TreeNode(): Node<T>(), childCount(0), childList(nullptr) {};

template <typename T>
TreeNode<T>::TreeNode(NodeType type_, T* data_): Node<T>(type_, data_),
                                                 childCount(0), childList(nullptr){};

template <typename T>
TreeNode<T>::TreeNode(const TreeNode &tn): Node<T>(tn.type, tn.data),
                                           childCount(tn.childCount), childList(tn.childList) {};

template <typename T>
TreeNode<T>::~TreeNode() {
  for (TreeNode** u = childList; u < u + childCount; u++)
    delete (*u);
}

template <typename T>
int TreeNode<T>::GetChildCount() const {
  return childCount;
}

template <typename T>
TreeNode<T>* TreeNode<T>::GetChild(int count) const {
  if (0 <= count && count < childCount )
    return childList[count];
  std::cout << "Error in TreeNode<T>* TreeNode<T>::GetChild(int count) const \n"
               "Index " << count << " out of range 0:" << childCount
            << std::endl;
  return nullptr;
}

template <typename T>
void TreeNode<T>::AppendChild(TreeNode<T> *node) {
  childCount++;
  childList[childCount] = node;
}


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

int main() {
//  Ingredient test
  std::string name = "potato";
  std::string unit = "kilogram";
  int count = 2;

  auto ingr1 = new Ingredient();
  auto ingr2 = new Ingredient(name, unit, count);
  auto ingr3 = new Ingredient(*ingr2);

  assert(ingr1->GetName() == " ");
  assert(ingr1->GetUnit() == " ");
  assert(ingr1->GetCount() == 0);

  assert(ingr2->GetName() == name);
  assert(ingr2->GetUnit() == unit);
  assert(ingr2->GetCount() == count);

  assert(ingr3->GetName() == name);
  assert(ingr3->GetUnit() == unit);
  assert(ingr3->GetCount() == count);

  unit = "pieces";
  ingr2->SetUnit(unit);
  assert(ingr2->GetUnit() == unit);

  count = 2;
  ingr2->SetCount(count);
  assert(ingr2->GetCount() == count);

//  Operation test
  std::string action = "cut";
  float time = 2.5;
  count = 2;

  auto op1 = new Operation();
  auto op2 = new Operation(action, time);
  auto op3 = new Operation(*op2);

  assert(op1->GetAction() == " ");
  assert(op1->GetTime() == 0);
  assert(op1->GetTime(count) == 0);

  assert(op2->GetAction() == action);
  assert(op2->GetTime() == time);
  assert(op2->GetTime(count) == time * count);

  assert(op3->GetAction() == action);
  assert(op3->GetTime() == time);
  assert(op3->GetTime(count) == count * time);

  time = 1.1;
  op2->SetTime(time);
  assert(op2->GetTime() == time);
  assert(op2->GetTime(count) == time * count);

  std::cout << "All tests are completed!"<< std::endl;
  return 0;
}
