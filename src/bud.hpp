#ifndef TREES_SRC_BUD_H
#define TREES_SRC_BUD_H

#include <glm/glm.hpp>
#include <list>

namespace environment {

class Marker;
class MarkerSet;

}  // namespace environment

namespace tree {

class Node;

class Bud {
 private:
 public:
  Bud(Node* _parent, environment::MarkerSet& m, bool _terminal);

  Node* parent;  // to get the position
  environment::MarkerSet& marker_set;
  bool terminal{false};
  int new_internodes{0};
  long id{rand()};
  float Q{0};  // represents available space or light
  float internode_length{0.0};
  glm::vec3 optm_growth_direction{glm::vec3(0.0)};

  glm::vec3 position();
  glm::vec3 orientation();
  void setBudFate();
};

}  // namespace tree

#endif