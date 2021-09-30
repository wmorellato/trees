#ifndef TREES_SRC_TREE_H
#define TREES_SRC_TREE_H

#include "node.hpp"
#include "marker.hpp"
#include <list>
#include <memory>
#include <glm/vec3.hpp>

namespace tree {

class Tree {
 private:
  std::list<Node> nodes;
  
  void CreateSeedling();
 public:
  Tree(environment::MarkerSet &marker_set);

  void associateMarkers(Node &node);
  void runGrowthIteration(Node &node);

  const float r = 4.0;
  const float theta = 90.0;
  std::unique_ptr<Node> root;
  environment::MarkerSet &marker_set;
};

}  // namespace tree

#endif
