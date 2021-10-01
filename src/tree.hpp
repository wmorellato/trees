#ifndef TREES_SRC_TREE_H
#define TREES_SRC_TREE_H

#include "marker.hpp"
#include "node.hpp"
#include <glm/vec3.hpp>
#include <list>
#include <memory>

namespace tree {

class Tree {
 private:
  std::list<Node> nodes;

  void createSeedling();

 public:
  Tree(environment::MarkerSet& marker_set);

  void run(int iterations);
  void runGrowthIteration(Node& node);
  void computeMarkers(Node& node);
  float passQBasipetallyBH(Node& node);
  void passVAcropetallyBH(Node& node, float v);
  void growShoots(Node& node);

  void createNewMarkers(Node& node);
  void assignMarkers(Node& node);
  void removeMarkersFromZone(Node& node);

  const float r = 4.0;
  const float theta = 90.0;
  const float ro = 2.0;
  const float lambda{2.0};
  const glm::vec3 tropism{glm::vec3(0.0, 1.0, 0.0)};

  std::unique_ptr<Node> root;
  environment::MarkerSet& marker_set;
};

}  // namespace tree

#endif
