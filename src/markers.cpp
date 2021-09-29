#include "markers.hpp"
#include <glm/gtc/random.hpp>
#include <algorithm>

using namespace environment;

Marker::Marker(glm::vec3 point) { this->point = point; }

void Marker::Reset() {
  dirty = false;
  min_distance = std::numeric_limits<double>::min();
}

MarkerSet::MarkerSet() {}

void MarkerSet::Reset() {
  for (std::list<Marker>::iterator it = markers.begin();
       it != markers.end(); ++it) {
    it->Reset();
  }
}

void MarkerSet::AddToSphere(glm::vec3 center, float radius, int count) {
  for (int i = 0; i < count; i++) {
    glm::vec3 point = glm::ballRand(radius) + center;
    markers.push_back(Marker(point));
  }
}

void MarkerSet::RemoveFromSphere(glm::vec3 center, float radius) {
  markers.remove_if([center, radius](const Marker& m) {
    return glm::distance(m.point, center) < radius;
  });
}
