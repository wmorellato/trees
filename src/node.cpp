#include "node.hpp"

using namespace tree;

Node::Node(Node* _parent, glm::vec3 _position, int _order) : parent(_parent), position(_position), order(_order) {
  if (_parent != nullptr) {
    this->direction = glm::normalize((this->position - _parent->position));
  } else {
    this->direction = glm::vec3(0.0, 1.0, 0.0);
  }
}

void Node::addBud(Bud b) {
  this->buds.push_back(b);
}

void Node::addChild(std::unique_ptr<Node> &n) {
  this->children.push_back(std::move(n));
}
