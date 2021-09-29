#include "node.hpp"

using namespace tree;
using namespace environment;

float Bud::GetQ() {
  // using space for now
  if (markers.size() > 0) {
    return 1.0;
  }
}

glm::vec3 Bud::GetPosition() { return this->parent->position; }

glm::vec3 Bud::GetOrientation() { return this->parent->direction; }

glm::vec3 Bud::GetOptimalGrowthDirection() {
  glm::vec3 ogd = glm::vec3(0.0);

  for (std::list<Marker*>::iterator it = markers.begin(); it != markers.end();
       ++it) {
    // the pointer may be null
    if ((*it) != nullptr) {
      ogd += (*it)->point;
    }
  }

  return glm::normalize(ogd);
}

Node::Node(Node* _parent, glm::vec3 _position, int _order) {
  if (_parent != nullptr) {
    this->direction = glm::normalize((this->position - _parent->position));
  }
}
