#ifndef TREES_SRC_BUD_H
#define TREES_SRC_BUD_H

#include <list>
#include <glm/glm.hpp>

namespace environment {

class Marker;

}

namespace tree {

class Node;

class Bud {
 private:
 public:
  Bud(Node* _parent, bool _terminal) : parent{_parent}, terminal{_terminal} {}

  Node* parent;  // to get the position
  long id = rand();
  float Q;  // represents available space or light
  bool terminal = false;
  std::list<environment::Marker*> markers;

  float getQ();
  glm::vec3 getPosition();
  glm::vec3 getOrientation();
  glm::vec3 getOptimalGrowthDirection();
};

}  // namespace tree

#endif