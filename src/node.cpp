#include "node.hpp"

using namespace tree;

Node::Node(Node* _parent, glm::vec3 _position, int _order)
    : parent{_parent}, position{_position}, order{_order} {
  if (parent != nullptr) {
    this->direction = glm::normalize((this->position - this->parent->position));
  } else {
    this->direction = glm::vec3(0.0, 1.0, 0.0);
  }
}

void Node::addChild(Node* n) {
  this->children.push_back(std::unique_ptr<Node>{n});
}

float Node::getQBuds() {
  float Q = 0.0;

  if (axillary_bud) Q += axillary_bud->Q;
  if (terminal_bud) Q += terminal_bud->Q;

  return Q;
}
