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
  Queue(): head(nullptr), tail(nullptr) {};
  ~Queue() {
    while (!IsEmpty()) {
      QueueNode* temp = head;
      head = head->next;

      if (temp->type == NodeType::Ingredient)
        delete static_cast<Ingredient*>(temp->data);
      else if (temp->type == NodeType::Operation)
        delete static_cast<Operation*>(temp->data);

      delete temp;
    }
  }
  template<typename T>
  void Enqueue(NodeType type, const T& item) {
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
  }

  template<class T>
  T Dequeue() {
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
  }


  [[nodiscard]] bool IsEmpty() const {
    return head == nullptr;
  }
  [[nodiscard]] QueueNode* GetHead() const {
    return head;
  }
 private:
  QueueNode* head;
  QueueNode* tail;
};






class Recipe {
 public:
  Recipe();
  Recipe(const std::string& name_, float time_, Queue* data_);
  Recipe(const Recipe &rc);
  ~Recipe();

  [[nodiscard]] std::string GetName() const;
  [[nodiscard]] float GetTime() const;
  virtual void Print() const;

 protected:
  std::string name;
  float time;
 private:
  Queue* data;
};

class RecipeTree final: public Recipe {
 public:
  void Print() const override;
 private:
  Tree* data;
};

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
}
std::string Recipe::GetName() const {
  return name;
}
float Recipe::GetTime() const {
  return time;
}
void Recipe::Print() const {
  QueueNode* curr;
  curr = data->GetHead();
  std::string outString;
  int currIngrCount = 1;
  while (curr != nullptr) {
    if (curr->type == NodeType::Ingredient) {
      outString = static_cast<Ingredient*>(curr->data)->Print();
      currIngrCount = static_cast<Ingredient*>(curr->data)->GetCount();
    } else if (curr->type == NodeType::Operation){
      outString = static_cast<Operation*>(curr->data)->Print(currIngrCount);
    }
    std::cout << outString <<std::endl;
    curr = curr->next;
  }
}



//template <typename T>
//TreeNode<T>::TreeNode(): Node<T>(), childCount(0), childList(nullptr) {};
//
//template <typename T>
//TreeNode<T>::TreeNode(NodeType type_, T* data_): Node<T>(type_, data_),
//                                                 childCount(0), childList(nullptr){};
//
//template <typename T>
//TreeNode<T>::TreeNode(const TreeNode &tn): Node<T>(tn.type, tn.data),
//                                           childCount(tn.childCount), childList(tn.childList) {};
//
//template <typename T>
//TreeNode<T>::~TreeNode() {
//  for (TreeNode** u = childList; u < u + childCount; u++)
//    delete (*u);
//}
//
//template <typename T>
//int TreeNode<T>::GetChildCount() const {
//  return childCount;
//}
//
//template <typename T>
//TreeNode<T>* TreeNode<T>::GetChild(int count) const {
//  if (0 <= count && count < childCount )
//    return childList[count];
//  std::cout << "Error in TreeNode<T>* TreeNode<T>::GetChild(int count) const \n"
//               "Index " << count << " out of range 0:" << childCount
//            << std::endl;
//  return nullptr;
//}
//
//template <typename T>
//void TreeNode<T>::AppendChild(TreeNode<T> *node) {
//  childCount++;
//  childList[childCount] = node;
//}

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
