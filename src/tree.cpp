#include "tree.hpp"
#include "spdlog/spdlog.h"
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
#include <cmath>
#include <iostream>

using namespace tree;
using namespace environment;

glm::vec3 getPointInSphereCap(glm::vec3 direction, float angle) {
  float rads = (std::rand() % (int)angle) * M_PI / 180.0;
  glm::vec2 point_circle = (glm::circularRand<double>(1.0)) * sin(rads);
  glm::vec3 V = glm::vec3(point_circle.x, point_circle.y, cos(rads));
  return glm::quatLookAt(direction, glm::vec3(0.0, 1.0, 0.0)) * V;
}

Tree::Tree(MarkerSet& marker_set) : marker_set{marker_set} { createSeedling(); }

void Tree::createSeedling() {
  root = std::make_unique<Node>(nullptr, glm::vec3(0.0), 0);
  root->addBud(Bud(root.get(), marker_set, true));
}

void Tree::run(int iterations) {
  for (int i = 0; i < iterations; i++) {
    runGrowthIteration(*root.get());
  }
}

void Tree::runGrowthIteration(Node& node) {
  if (&node == nullptr) {
    spdlog::error("tree is null");
    return;
  }

  computeMarkers(node);
  float Qbase = passQBasipetallyBH(node) * lambda;
  passVAcropetallyBH(node, Qbase);
  growShoots(node);
}

void Tree::computeMarkers(Node& node) {
  spdlog::info("node {} has {} buds and {} children", node.order,
               node.buds.size(), node.children.size());

  for (auto& b : node.buds) {
    marker_set.addToSphere(b.position() + glm::normalize(b.orientation()), 2.0,
                           10);
    marker_set.removeFromSphere(b.position(), ro);
    marker_set.assignBud(b.id, b.position(), r, theta);
    b.setBudFate();
  }

  std::list<std::unique_ptr<Node>>::iterator it;
  for (it = node.children.begin(); it != node.children.end(); ++it) {
    computeMarkers(*(it->get()));
  }
}

float Tree::passQBasipetallyBH(Node& node) {
  float Qm = 0.0f;
  float Ql = 0.0f;

  std::list<std::unique_ptr<Node>>::iterator it;
  for (it = node.children.begin(); it != node.children.end(); ++it) {
    if (!(*it)) {
      spdlog::error("null child");
      continue;
    }

    Node* child = it->get();

    if (child->order == node.order) {
      Qm += passQBasipetallyBH(*child);
    } else {
      Ql += passQBasipetallyBH(*child);
    }
  }

  node.Qm = Qm;
  node.Ql = Ql;

  return node.Ql + node.Qm + node.getQBuds();
}

void Tree::passVAcropetallyBH(Node& node, float v) {
  node.v = v;

  float vm =
      v * (lambda * node.Qm / (lambda * node.Qm + (1 - lambda) * node.Ql));
  float vl = v * ((1 - lambda) * node.Ql /
                  (lambda * node.Qm + (1 - lambda) * node.Ql));

  for (auto& b : node.buds) {
    b.new_internodes = floorf(v);
    b.internode_length = v / b.new_internodes;
  }

  std::list<std::unique_ptr<Node>>::iterator it;
  for (it = node.children.begin(); it != node.children.end(); ++it) {
    if (!(*it)) {
      continue;
    }

    Node* child = it->get();

    if (child->order == node.order) {
      passVAcropetallyBH(*child, vm);
    } else {
      passVAcropetallyBH(*child, vl);
    }
  }
}

void Tree::growShoots(Node& node) {
  for (auto& b : node.buds) {
    if (b.new_internodes > 0) {
      glm::vec3 direction;
      int order;

      if (b.terminal) {
        direction =
            glm::normalize(node.direction + b.optm_growth_direction + tropism);
        order = node.order;
      } else {
        direction = glm::normalize(getPointInSphereCap(node.direction, 90.0));
        order = node.order++;
      }

      Node* n = std::addressof(node);
      for (int i = 0; i < b.new_internodes; i++) {
        Node* new_node =
            new Node(n, n->position + direction * b.internode_length, order);
        new_node->addBud(Bud(n, marker_set, false));

        n->addChild(new_node);
        n = new_node;
      }

      n->addBud(Bud(n, marker_set, true));
    }
  }

  node.buds.clear();
}

// ---

void Tree::createNewMarkers(Node& node) {
  for (auto b : node.buds) {
    marker_set.addToSphere(b.position() + glm::normalize(b.orientation()), 2.0,
                           10);
  }
}

void Tree::assignMarkers(Node& node) {
  for (std::list<Bud>::iterator it = node.buds.begin(); it != node.buds.end();
       ++it) {
    marker_set.assignBud(it->id, it->position(), r, theta);
  }

  for (std::list<std::unique_ptr<Node>>::iterator it = node.children.begin();
       it != node.children.end(); ++it) {
    // I'm screwed if there are circular dependencies between nodes...
    assignMarkers(*(it->get()));
  }
}

void Tree::removeMarkersFromZone(Node& node) {
  for (auto b : node.buds) {
    marker_set.removeFromSphere(b.position(), ro);
    marker_set.addToSphere(b.position() + glm::normalize(b.orientation()), 2.0,
                           10);
  }
}