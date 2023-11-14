#ifndef LAB1__QUEUE_H_
#define LAB1__QUEUE_H_

#include "NodeType.h"

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

#endif //LAB1__QUEUE_H_
