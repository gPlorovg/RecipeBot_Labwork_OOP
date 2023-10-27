#include <string>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <vector>

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

struct QueueNode {
  NodeType type;
  void* data;
  QueueNode* next;
};

class Queue {
 public:
  Queue();
  ~Queue();
  template<typename T>
  void Enqueue(NodeType type, const T& item);

  template<typename T>
  T Dequeue();

  [[nodiscard]] bool IsEmpty() const;
  [[nodiscard]] QueueNode* GetHead() const;

 private:
  QueueNode* head;
  QueueNode* tail;
};

struct TreeNode {
  NodeType type;
  void* data;
  std::vector<TreeNode*> children;
};

class Tree{
 public:
  Tree();
  ~Tree();

  void AddNode(const Ingredient& item);
  void AddNode(const Operation& item);
  [[nodiscard]] TreeNode* GetRoot() const;
  std::string Print();

 private:
  TreeNode* curr;
  TreeNode* root;
  std::stringstream stream;

  TreeNode* FindNode(const Operation& op);

  template<typename T>
  TreeNode* AppendChildNode(NodeType type, const T& item);

  void Out(TreeNode *tn, int margin, bool isFirst);
  void Delete(TreeNode *tn);
};

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

Queue::Queue(): head(nullptr), tail(nullptr) {};
Queue::~Queue() {
  while (!IsEmpty()) {
    QueueNode* temp = head;
    head = head->next;

    if (temp->type == NodeType::Ingredient)
      delete static_cast<Ingredient*>(temp->data);
    else if (temp->type == NodeType::Operation)
      delete static_cast<Operation*>(temp->data);

    delete temp;
  }
  head = nullptr;
  tail = nullptr;
};
template<typename T>
void Queue::Enqueue(NodeType type, const T &item) {
  QueueNode* newNode = new QueueNode;
  newNode->type = type;
  newNode->data = new T(item);
  newNode->next = nullptr;

  if (IsEmpty()) {
    head = tail = newNode;
  } else {
    tail->next = newNode;
    tail = newNode;
  }
};
template<typename T>
T Queue::Dequeue() {
  if (IsEmpty()) {
    throw std::runtime_error("Queue is empty!");
  }

  T item = *static_cast<T*>(head->data);
  QueueNode* temp = head;
  head = head->next;
  delete static_cast<T*>(temp->data);
  delete temp;

  if (head == nullptr) {
    tail = nullptr;
  }

  return item;
};
bool Queue::IsEmpty() const {
  return head == nullptr;
};
QueueNode* Queue::GetHead() const {
  return head;
};

Tree::Tree() {
  root = new TreeNode{NodeType::None, nullptr};
  curr = root;
};
Tree::~Tree() {
  Delete(root);
};
void Tree::AddNode(const Ingredient &item) {
  AppendChildNode(NodeType::Ingredient, item);
  curr = root;
};
void Tree::AddNode(const Operation &item) {
  TreeNode* tmp = FindNode(item);
  if (tmp == nullptr)
    curr = AppendChildNode(NodeType::Operation, item);
  else
    curr = tmp;
};
TreeNode* Tree::GetRoot() const {
  return root;
};
std::string Tree::Print() {
  Out(root, 0, true);
  return stream.str();
}
TreeNode* Tree::FindNode(const Operation& op) {
  for (TreeNode* tn : curr->children) {
    std::string s = static_cast<Operation*>(tn->data)->GetAction();
    if (s == op.GetAction())
      return tn;
  }
  return nullptr;
};
template<typename T>
TreeNode* Tree::AppendChildNode(NodeType type, const T &item) {
  auto tn = new TreeNode;
  tn->type = type;
  tn->data = new T(item);
  curr->children.push_back(tn);
  return tn;
};
void Tree::Out(TreeNode *tn, int margin, bool isFirst) {
  std::string outString;
  if (tn->type == NodeType::Ingredient)
    outString = static_cast<Ingredient*>(tn->data)->Print();
  else if (tn->type == NodeType::Operation)
    outString = static_cast<Operation*>(tn->data)->Print() + " <- ";
  if (!isFirst)
    stream<< std::setfill(' ') << std::setw(margin + outString.length()) <<
          outString;
  else
    stream << outString;
  if (!tn->children.empty())
    for(TreeNode* u : tn->children)
      Out(u, margin + outString.length(),
          u == tn->children.front());
  else
    stream << std::endl;
};
void Tree::Delete(TreeNode *tn) {
  if (tn == nullptr)
    return;
  else {
    for(TreeNode* u : tn->children)
      Delete(u);
    if (tn->type == NodeType::Ingredient)
      delete static_cast<Ingredient*>(tn->data);
    else if (tn->type == NodeType::Operation)
      delete static_cast<Operation*>(tn->data);
    else if (tn->type == NodeType::None)
      return;
    delete tn;
  }
}

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

int main() {
//  Ingredient test
//  std::string name = "potato";
//  std::string unit = "kilogram";
//  int count = 2;
//
//  auto ingr1 = new Ingredient();
//  auto ingr2 = new Ingredient(name, unit, count);
//  auto ingr3 = new Ingredient(*ingr2);
//
//  assert(ingr1->GetName() == " ");
//  assert(ingr1->GetUnit() == " ");
//  assert(ingr1->GetCount() == 0);
//
//  assert(ingr2->GetName() == name);
//  assert(ingr2->GetUnit() == unit);
//  assert(ingr2->GetCount() == count);
//
//  assert(ingr3->GetName() == name);
//  assert(ingr3->GetUnit() == unit);
//  assert(ingr3->GetCount() == count);
//
//  unit = "pieces";
//  ingr2->SetUnit(unit);
//  assert(ingr2->GetUnit() == unit);
//
//  count = 2;
//  ingr2->SetCount(count);
//  assert(ingr2->GetCount() == count);

//  Operation test
//  std::string action = "cut";
//  float time = 2.5;
//  count = 2;
//
//  auto op1 = new Operation();
//  auto op2 = new Operation(action, time);
//  auto op3 = new Operation(*op2);
//
//  assert(op1->GetAction() == " ");
//  assert(op1->GetTime() == 0);
//  assert(op1->GetTime(count) == 0);
//
//  assert(op2->GetAction() == action);
//  assert(op2->GetTime() == time);
//  assert(op2->GetTime(count) == time * count);
//
//  assert(op3->GetAction() == action);
//  assert(op3->GetTime() == time);
//  assert(op3->GetTime(count) == count * time);
//
//  time = 1.1;
//  op2->SetTime(time);
//  assert(op2->GetTime() == time);
//  assert(op2->GetTime(count) == time * count);
//
//  std::cout << "All tests are completed!"<< std::endl;
//  return 0;
  auto ing1 = Ingredient("potato", "kg", 10);
  auto ing2 = Ingredient("onion", "unit", 6);
  auto ing3 = Ingredient("carrot", "unit", 2);

  auto op1 = Operation("chop", 2);
  auto op2 = Operation("put", 0.1);
  auto op3 = Operation("boil", 100);
  auto op4 = Operation("fry", 50);
//  Queue Recipe test
  Queue q;

  q.Enqueue(NodeType::Ingredient, ing1);
  q.Enqueue(NodeType::Operation, op1);
  q.Enqueue(NodeType::Operation, op3);
  q.Enqueue(NodeType::Operation, op2);
  q.Enqueue(NodeType::Ingredient, ing2);
  q.Enqueue(NodeType::Operation, op1);
  q.Enqueue(NodeType::Operation, op4);
  q.Enqueue(NodeType::Operation, op2);
  std::string name = "Borshch";
  float time = 36000.30;
  auto r = new Recipe(name, time, &q);
  assert(r->GetName() == name);
  assert(r->GetTime() == time);

  std::cout << r->Print();

// Tree Recipe test
  Tree tr;

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

  name = "Salad";
  time = 200;
  auto rt = new RecipeTree(name, time, &tr);
  std::cout << rt->Print();

  assert(rt->GetName() == name);
  assert(rt->GetTime() == time);

  std::cout << std::endl;
  std::cout << "All tests are completed!"<< std::endl;
  return 0;
}
