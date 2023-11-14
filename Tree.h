#ifndef LAB1__TREE_H_
#define LAB1__TREE_H_

#include <vector>
#include <sstream>
#include "Ingredient.h"
#include "Operation.h"
#include "NodeType.h"

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
#endif //LAB1__TREE_H_
