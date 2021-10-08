#include "tree.hpp"
#include "spdlog/spdlog.h"
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
#include <cmath>
#include <iostream>

using namespace tree;
using namespace environment;

std::string pv3(glm::vec3 v) {
  std::ostringstream os;
  os << v.x << "," << v.y << "," << v.z;
  return os.str();
}

glm::vec3 getPointInSphereCap(glm::vec3 direction, float angle) {
  double rads = (std::rand() % (int)angle) * M_PI / 180.0;
  glm::vec2 point_circle = glm::normalize(glm::circularRand<double>(1.0)) * sin(rads);
  glm::vec3 V = glm::vec3(point_circle.x, point_circle.y, cos(rads));
  return glm::quatLookAtLH(glm::normalize(direction), glm::vec3(0.0, 1.0, 0.0)) * V;
}

Tree::Tree(MarkerSet& marker_set) : marker_set{marker_set} { createSeedling(); }

void Tree::createSeedling() {
  root = std::make_unique<Node>(nullptr, glm::vec3(0.0), 0);
  root->level = 1;
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
  float Qbase = passQBasipetallyBH(node) * environment::alpha;

  if (Qbase > environment::qbase_max) {
    return;
  }

  passVAcropetallyBH(node, Qbase);
  growShoots(node);
}

void Tree::computeMarkers(Node& node) {
  // spdlog::info("node {} has {} buds and {} children", node.level,
  //              node.buds.size(), node.children.size());

  for (auto& b : node.buds) {
    marker_set.addToSphere(b.position(), 4.0, environment::num_markers);
    marker_set.removeFromSphere(b.position(), environment::ro);
    marker_set.assignBud(b.id, b.position(), environment::r,
                         environment::theta);
    b.setBudFate();
  }

  std::list<std::unique_ptr<Node>>::iterator it;
  for (it = node.children.begin(); it != node.children.end(); ++it) {
    Node& foo = *(it->get());
    computeMarkers(foo);
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

  for (auto b : node.buds) {
    if (b.terminal) {
      Qm += b.Q;
    } else {
      Ql += b.Q;
    }
  }

  node.Qm = Qm;
  node.Ql = Ql;

  return node.Ql + node.Qm;
}

void Tree::passVAcropetallyBH(Node& node, float v) {
  node.v = v;
  float l = environment::lambda;

  float vm = v * (l * node.Qm) / (l * node.Qm + (1 - l) * node.Ql);
  float vl = v * ((1 - l) * node.Ql) / (l * node.Qm + (1 - l) * node.Ql);

  for (auto& b : node.buds) {
    b.new_internodes = floorf(v);
    if (b.internode_length > 2) b.internode_length = 2;
    b.internode_length = v / b.new_internodes;
  }

  // if (node.children.size() < 2) {
  //   // it's not a branching point
  //   vm = v;
  // }

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
  spdlog::info("node {}, buds {}, children {}, parent {}", node.id,
               node.buds.size(), node.children.size(), (void*)node.parent);

  for (auto& c : node.children) {
    growShoots(*c.get());
  }

  for (auto& b : node.buds) {
    if (b.new_internodes > 0) {
      glm::vec3 direction;
      int order;

      Node* n = std::addressof(node);
      for (int i = 0; i < b.new_internodes; i++) {

        if (b.terminal) {
          glm::vec3 randv(getPointInSphereCap(node.direction, 60.0));
          direction = glm::normalize(node.direction) + glm::normalize(b.optm_growth_direction) + randv + environment::tropism();
          order = node.order;
        } else {
          glm::vec3 randv(getPointInSphereCap(node.direction, 90.0));
          direction = randv;
          order = node.order + 1;
        }

        Node* new_node = new Node(n, n->position + direction*b.internode_length, order);
        new_node->addBud(Bud(n, marker_set, false));

        n->addChild(new_node);
        n = new_node;
      }

      n->addBud(Bud(n, marker_set, true));
      n->addBud(Bud(n, marker_set, false));
    }
  }

  node.buds.clear();
}
