#include <glm/glm.hpp>
#include <limits>
#include <vector>

namespace environment {

class Marker {
 private:
 public:
  glm::vec3 point;
  bool dirty = false;
  double min_distance = std::numeric_limits<double>::min();

  Marker(glm::vec3 point);
  
  void Reset();
};

class MarkerSet {
 private:
 public:
  std::vector<Marker> markers;

  MarkerSet();

  void Reset();
  void AddToSphere(glm::vec3 center, float radius, int count);
};

}  // namespace environment
