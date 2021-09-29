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
  for (std::vector<Marker>::iterator it = std::begin(markers);
       it != std::end(markers); ++it) {
    it->Reset();
  }
}

void MarkerSet::AddToSphere(glm::vec3 center, float radius, int count) {
  for (int i = 0; i < count; i++) {
    glm::vec3 point = glm::ballRand(radius) + center;
    Marker m(point);
    markers.emplace_back(m);
  }
}

void MarkerSet::RemoveFromSphere(glm::vec3 center, float radius) {
  std::vector<Marker>::iterator pend = std::remove_if(markers.begin(), markers.end(), [center, radius](const Marker& m) {
    return glm::distance(m.point, center) < radius;
  });

  markers.erase(pend, markers.end());
}