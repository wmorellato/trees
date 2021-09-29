#ifndef TREES_SRC_NODE_H
#define TREES_SRC_NODE_H

#include <glm/glm.hpp>
#include <list>
#include "markers.hpp"

namespace tree {

class Bud {
 private:
 public:
  Bud(Node* _parent, bool _terminal) : parent{_parent}, terminal{_terminal} {}

  Node* parent; // to get the position
  bool terminal = false;
  float Q; // represents available space or light
  std::list<environment::Marker*> markers;

  float GetQ();
  glm::vec3 GetPosition();
  glm::vec3 GetOrientation();
  glm::vec3 GetOptimalGrowthDirection();
};

class Node {
 private:
 public:
  Node(Node* _parent, glm::vec3 _position, int _order)
      : parent{_parent}, position{_position}, order{_order} {}

  Node* parent;
  glm::vec3 position;
  glm::vec3 direction;
  int order;
  float branch_diameter = 0.01;
  float Qm = 0.0;  // resource from main branch
  float Ql = 0.0;  // resource from lateral branch
  float v = 0.0;   // resource allocated
};

}  // namespace tree

#endif
