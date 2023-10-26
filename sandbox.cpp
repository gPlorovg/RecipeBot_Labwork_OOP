#include <string>
#include <assert.h>
#include <iostream>
#include <vector>
#include <iomanip>

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


enum class NodeType {None, Ingredient, Operation};

struct TreeNode {
  NodeType type;
  void* data;
  std::vector<TreeNode*> children;
};

class Tree{
 public:
  Tree() {
    root = new TreeNode{NodeType::None, nullptr};
    curr = root;
  };
  void AddNode(const Ingredient& item) {
    AppendChildNode(NodeType::Ingredient, item);
    curr = root;
    }
  void AddNode(const Operation& item) {
    TreeNode* tmp = FindNode(curr, item);
    if (tmp == nullptr)
      curr = AppendChildNode(NodeType::Operation, item);
    else
      curr = tmp;
  }

  [[nodiscard]] bool IsFull() const{
    return curr == root;
  }

  static TreeNode* FindNode(TreeNode* parent, const Operation& op) {
    if (parent != nullptr) {
      for (TreeNode* tn : parent->children) {
        std::string s = static_cast<Operation*>(tn->data)->GetAction();
        if (s == op.GetAction())
          return tn;
      }
    }
    return nullptr;
  }

  [[nodiscard]] TreeNode* GetRoot() const {
    return root;
  }

 private:
  TreeNode* curr;
  TreeNode* root;
  template<class T>
  TreeNode* AppendChildNode(NodeType type, const T& item) {
    auto tn = new TreeNode;
    tn->type = type;
    tn->data = new T(item);

    curr->children.push_back(tn);

    return tn;
  }
};

void print(TreeNode *tn, int margin, bool isFirst) {
  std::string outString;
  if (tn->type == NodeType::Ingredient)
    outString = static_cast<Ingredient*>(tn->data)->Print();
  else if (tn->type == NodeType::Operation)
    outString = static_cast<Operation*>(tn->data)->Print() + " <- ";
  if (!isFirst)
    std::cout << std::setfill(' ') << std::setw(margin + outString.length()) <<
    outString;
  else
    std::cout << outString;
  if (!tn->children.empty())
    for(TreeNode* u : tn->children)
      print(u, margin + outString.length(),
            u == tn->children.front());
  else
    std::cout << std::endl;
}

int main() {
  Tree tr;
  auto ing1 = Ingredient("potato", "kg", 10);
  auto ing2 = Ingredient("onion", "unit", 6);
  auto ing3 = Ingredient("carrot", "unit", 2);
  auto op1 = Operation("chop", 2);
  auto op2 = Operation("put", 0.1);
  auto op3 = Operation("boil", 100);
  auto op4 = Operation("fry", 50);
  tr.AddNode(op2);
  tr.AddNode(op3);
  tr.AddNode(op1);
  tr.AddNode(ing1);

  tr.AddNode(op2);
  tr.AddNode(op4);
  tr.AddNode(op1);
  tr.AddNode(ing2);

  tr.AddNode(op2);
  tr.AddNode(op4);
  tr.AddNode(op1);
  tr.AddNode(ing3);

  tr.AddNode(op4);
  tr.AddNode(op1);
  tr.AddNode(ing1);

  print(tr.GetRoot(), 0, true);
  return 0;
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
