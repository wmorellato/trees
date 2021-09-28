#include "markers.hpp"
#include <glm/gtc/random.hpp>

using namespace environment;

Marker::Marker(glm::vec3 point) { this->point = point; }

void Marker::Reset() {
  dirty = false;
  min_distance = std::numeric_limits<double>::min();
}

MarkerSet::MarkerSet() {}

void MarkerSet::Reset() {
  for (std::vector<Marker>::iterator it = std::begin(markers);
       it != std::end(markers); ++it) {
    it->Reset();
  }
}

void MarkerSet::AddToSphere(glm::vec3 center, float radius, int count) {
  for (int i = 0; i < count; i++) {
    glm::vec3 point = glm::sphericalRand(radius) + center;
    Marker* m = new Marker(point);
  }
}
