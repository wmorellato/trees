#ifndef TREES_SRC_NODE_H
#define TREES_SRC_NODE_H

#include "bud.hpp"
#include <memory>

namespace tree {

class Node {
 private:
 public:
  Node(Node* _parent, glm::vec3 _position, int _order);

  int order;
  float branch_diameter = 0.01;
  float Qm = 0.0;  // resource from main branch
  float Ql = 0.0;  // resource from lateral branch
  float v = 0.0;   // resource allocated
  Node* parent;
  glm::vec3 position;
  glm::vec3 direction;
  std::list<Bud> buds;
  std::list<std::unique_ptr<Node>> children;

  void addBud(Bud b);
  void addChild(std::unique_ptr<Node> &n);
};

}  // namespace tree

#endif
