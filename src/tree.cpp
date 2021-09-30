#include "tree.hpp"

using namespace tree;
using namespace environment;

Tree::Tree(MarkerSet& marker_set) : marker_set{marker_set} {
  CreateSeedling();
  marker_set.addToSphere(root->position, 1.0f, 10);
}

void Tree::CreateSeedling() {
  // Node root(nullptr, glm::vec3(0.0), 0);
  // root.AddBud(Bud(&root, true));
  // nodes.push_back(root);
  root = std::make_unique<Node>(nullptr, glm::vec3(0.0), 0);
  root->addBud(Bud(root.get(), true));
}

void Tree::runGrowthIteration(Node& node) {
  if (&node == nullptr) {
    return;
  }

  associateMarkers(node);
}

void Tree::associateMarkers(Node& node) {
  for (std::list<Bud>::iterator it = node.buds.begin(); it != node.buds.end();
       ++it) {
    marker_set.assignBud(it->id, it->getPosition(), r, theta);
  }

  for (std::list<std::unique_ptr<Node>>::iterator it = node.children.begin();
       it != node.children.end(); ++it) {
    // I'm screwed if there are circular dependencies between nodes...
    associateMarkers(*(it->get()));
  }
}
