#ifndef TREES_SRC_MARKERS_H
#define TREES_SRC_MARKERS_H

#include <glm/glm.hpp>
#include <iostream>
#include <limits>
#include <list>

namespace environment {

class Marker {
 private:
 public:
  explicit Marker(glm::vec3 _position);
  void reset();

  glm::vec3 position;
  long bud_id;
  float min_distance = std::numeric_limits<float>::infinity();

  friend std::ostream& operator<<(std::ostream& os, Marker const& m) {
    return os << "x: " << m.position.x << " y: " << m.position.y
              << " z: " << m.position.z << std::endl;
  }
};

class MarkerSet {
 private:
 public:
  std::list<Marker> markers;
  std::list<Marker*> dirty_markers;

  MarkerSet();

  void reset();
  void addToSphere(glm::vec3 center, float radius, int count);
  void removeFromSphere(glm::vec3 venter, float radius);
  void assignBud(long bud_id, glm::vec3 bud_position, float r, float theta);
  glm::vec3 getGrowthDirection(long bud_id, glm::vec3 position);
};

}  // namespace environment

#endif
