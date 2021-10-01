#include "bud.hpp"
#include "marker.hpp"
#include "node.hpp"

using namespace tree;

Bud::Bud(Node* _parent, environment::MarkerSet& m, bool _terminal)
    : parent{_parent}, marker_set{m}, terminal{_terminal} {}

glm::vec3 Bud::position() { return parent->position; }

glm::vec3 Bud::orientation() { return parent->direction; }

void Bud::setBudFate() {
  glm::vec3 ogd = glm::vec3(0.0);

  for (environment::Marker* m : marker_set.dirty_markers) {
    if (m->bud_id == id) {
      Q = 1.0;
      optm_growth_direction += glm::normalize(m->position - parent->position);
    }
  }
}
