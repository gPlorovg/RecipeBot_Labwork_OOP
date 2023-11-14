#include <string>
#include <assert.h>
#include <iostream>

#include "Ingredient.h"
#include "Operation.h"
#include "NodeType.h"
#include "Queue.h"
#include "Tree.h"
#include "Recipe.h"

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


int main() {
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
  std::string test_rez = "Borshch | 36000.3\n"
                         "[10 kg of potato]\n"
                         "(chop : 20.000000)\n"
                         "(boil : 1000.000000)\n"
                         "(put : 1.000000)\n"
                         "[6 unit of onion]\n"
                         "(chop : 12.000000)\n"
                         "(fry : 300.000000)\n"
                         "(put : 0.600000)\n";
  assert(r->Print() == test_rez);
//  std::cout << r->Print();

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

  assert(rt->GetName() == name);
  assert(rt->GetTime() == time);
  test_rez = "Salad | 200\n"
             "(put : 0.100000) <- (boil : 100.000000) <- (chop : 2.000000) <- [10 kg of potato]\n"
             "                    (fry : 50.000000) <- (chop : 2.000000) <- [6 unit of onion]\n"
             "                                                              [2 unit of carrot]\n"
             "(fry : 50.000000) <- (chop : 2.000000) <- [10 kg of potato]\n";
  assert(rt->Print() == test_rez);
//  std::cout << rt->Print();

  std::cout << std::endl;
  std::cout << "All tests are completed!"<< std::endl;
  return 0;
}
