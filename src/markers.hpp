#ifndef TREES_SRC_MARKERS_H
#define TREES_SRC_MARKERS_H

#include <glm/glm.hpp>
#include <limits>
#include <list>
#include <iostream>

namespace environment {

class Marker {
 private:
 public:
  glm::vec3 point;
  bool dirty = false;
  double min_distance = std::numeric_limits<double>::min();

  Marker(glm::vec3 point);
  
  void Reset();

  friend std::ostream &operator<<(std::ostream &os, Marker const &m) {
    return os << "x: " << m.point.x << " y: " << m.point.y << " z: " << m.point.z << std::endl;
  }
};

class MarkerSet {
 private:
 public:
  std::list<Marker> markers;

  MarkerSet();

  void Reset();
  void AddToSphere(glm::vec3 center, float radius, int count);
  void RemoveFromSphere(glm::vec3 venter, float radius);
};

}  // namespace environment

#endif
