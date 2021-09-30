#include "bud.hpp"
#include "marker.hpp"
#include "node.hpp"

using namespace tree;

float Bud::getQ() {
  // using space for now
  if (markers.size() > 0) {
    return 1.0;
  }

  return 0.0;
}

glm::vec3 Bud::getPosition() { return parent->position; }

glm::vec3 Bud::getOrientation() { return parent->direction; }

glm::vec3 Bud::getOptimalGrowthDirection() {
  glm::vec3 ogd = glm::vec3(0.0);

  for (std::list<environment::Marker*>::iterator it = markers.begin(); it != markers.end();
       ++it) {
    // the pointer may be null
    if ((*it) != nullptr) {
      ogd += (*it)->position;
    }
  }

  return glm::normalize(ogd);
}
