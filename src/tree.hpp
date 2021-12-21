#ifndef TREES_SRC_TREE_H
#define TREES_SRC_TREE_H

#include "environment.hpp"

// to be removed
#include <easy3d/renderer/drawable_lines.h>
#include <easy3d/renderer/drawable_points.h>
#include <easy3d/renderer/camera.h>
#include <easy3d/core/types.h>
#include <easy3d/fileio/resources.h>
#include <easy3d/viewer/viewer.h>

#include "marker.hpp"
#include "node.hpp"
#include <glm/vec3.hpp>
#include <list>
#include <memory>

glm::vec3 getPointInSphereCap(glm::vec3 direction, float angle);

namespace tree {

class Tree {
 private:
  std::list<Node> nodes;

  void createSeedling();

 public:
  explicit Tree(environment::MarkerSet& _marker_set);

  void run(int iterations);
  void runGrowthIteration(Node& node);
  void computeMarkers(Node& node);
  float passQBasipetallyBH(Node& node);
  void passVAcropetallyBH(Node& node, float v);
  void growShoots(Node& node);

  void createNewMarkers(Node& node);
  void assignMarkers(Node& node);
  void removeMarkersFromZone(Node& node);

  std::unique_ptr<Node> root;
  environment::MarkerSet& marker_set;
  environment::ShadowGrid shadow_grid;
};

}  // namespace tree

#endif
