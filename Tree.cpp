#include <iomanip>
#include "Tree.h"

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