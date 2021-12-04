#ifndef TREES_SRC_NODE_H
#define TREES_SRC_NODE_H

#include "bud.hpp"
#include <memory>

namespace tree {

class Node {
 private:
 public:
  Node(Node* _parent, glm::vec3 _position, int _order);

  Node* parent;
  glm::vec3 position;
  int order;
  int level;
  long id{rand()};
  float branch_diameter = 0.01f;
  float Qbuds = 0.0f;
  float Qm = 0.0f;  // resource from main branch
  float Ql = 0.0f;  // resource from lateral branch
  float v = 0.0f;   // resource allocated
  glm::vec3 direction;
  Bud* axillary_bud = NULL;
  Bud* terminal_bud = NULL;
  std::list<std::unique_ptr<Node>> children;

  void addChild(Node* n);
  float getQBuds();
};

}  // namespace tree

#endif
