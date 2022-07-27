#include "marker.hpp"
#include <glm/gtc/random.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <algorithm>

using namespace environment;

Marker::Marker(glm::vec3 _position) : position(_position) {}

void Marker::reset() {
  bud_id = 0;
  min_distance = std::numeric_limits<float>::max();
}

MarkerSet::MarkerSet() {}

void MarkerSet::reset() {
  dirty_markers.clear();

  for (std::list<Marker>::iterator it = markers.begin(); it != markers.end();
       ++it) {
    it->reset();
  }
}

void MarkerSet::addToSphere(glm::vec3 center, float radius, int count) {
  for (int i = 0; i < count; i++) {
    glm::vec3 position = glm::ballRand(radius) + center;
    markers.push_back(Marker(position));
  }
}

void MarkerSet::removeFromSphere(glm::vec3 center, float radius) {
  markers.remove_if([center, radius](const Marker& m) {
    return glm::distance(m.position, center) < radius;
  });
}

void MarkerSet::assignBud(long bud_id, glm::vec3 bud_position, float r,
                          float theta) {
  // temporary
  for (std::list<Marker>::iterator it = markers.begin(); it != markers.end();
       ++it) {
    Marker* m = &(*it);
    float distance = glm::distance(m->position, bud_position);
    float angle = glm::angle(m->position, bud_position);

    if (distance < r && distance < m->min_distance) {
      if (angle < theta / 2.0f) {
        m->bud_id = bud_id;
        m->min_distance = distance;
        dirty_markers.push_back(m);
      }
    }
  }
}

glm::vec3 MarkerSet::getGrowthDirection(long bud_id, glm::vec3 position) {
  glm::vec3 growth_direction(0.0, 0.0, 0.0);

  for (auto m : markers) {
    if (m.bud_id == bud_id) {
      growth_direction += glm::normalize(m.position - position);
    }
  }

  return glm::normalize(growth_direction);
}
