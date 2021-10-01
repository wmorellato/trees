#include "node.hpp"

using namespace tree;

Node::Node(Node* parent, glm::vec3 position, int order)
    : parent{parent}, position{position}, order{order} {
  if (parent != nullptr) {
    this->direction = glm::normalize((this->position - this->parent->position));
  } else {
    this->direction = glm::vec3(0.0, 1.0, 0.0);
  }
}

void Node::addBud(const Bud& b) { this->buds.push_back(b); }

void Node::addChild(Node* n) {
  this->children.push_back(std::unique_ptr<Node>{n});
}

float Node::getQBuds() {
  float Q = 0.0;

  if (buds.size() == 0) {
    return Q;
  }

  for (auto b : buds) {
    Q += b.Q;
  }

  return Q;
}
