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
  int level;
  long id{rand()};
  float branch_diameter = 0.01;
  float Qbuds = 0.0;
  float Qm = 0.0;  // resource from main branch
  float Ql = 0.0;  // resource from lateral branch
  float v = 0.0;   // resource allocated
  Node* parent;
  glm::vec3 position;
  glm::vec3 direction;
  std::list<Bud> buds;
  std::list<std::unique_ptr<Node>> children;

  void addBud(const Bud& b);
  void addChild(Node* n);
  float getQBuds();
};

}  // namespace tree

#endif
