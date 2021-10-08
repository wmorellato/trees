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
  root->terminal_bud = new Bud(root.get(), marker_set, true);
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

  if (node.terminal_bud) {
    marker_set.addToSphere(node.terminal_bud->position(), 4.0, environment::num_markers);
    marker_set.removeFromSphere(node.terminal_bud->position(), environment::ro);
    marker_set.assignBud(node.terminal_bud->id, node.terminal_bud->position(), environment::r,
                          environment::theta);
    node.terminal_bud->setBudFate();
  }

  if (node.axillary_bud) {
    marker_set.addToSphere(node.axillary_bud->position(), 4.0, environment::num_markers);
    marker_set.removeFromSphere(node.axillary_bud->position(), environment::ro);
    marker_set.assignBud(node.axillary_bud->id, node.axillary_bud->position(), environment::r,
                          environment::theta);
    node.axillary_bud->setBudFate();
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

  if (node.terminal_bud) Qm = node.terminal_bud->Q;
  if (node.axillary_bud) Ql = node.axillary_bud->Q;

  node.Qm = Qm;
  node.Ql = Ql;

  return node.Ql + node.Qm;
}

void Tree::passVAcropetallyBH(Node& node, float v) {
  node.v = v;
  float l = environment::lambda;

  float vm = v * (l * node.Qm) / (l * node.Qm + (1 - l) * node.Ql);
  float vl = v * ((1 - l) * node.Ql) / (l * node.Qm + (1 - l) * node.Ql);

  if (node.terminal_bud) {
    node.terminal_bud->new_internodes = floorf(v);
    node.terminal_bud->internode_length = v / node.terminal_bud->new_internodes;
  }

  if (node.axillary_bud) {
    node.axillary_bud->new_internodes = floorf(v);
    node.axillary_bud->internode_length = v / node.axillary_bud->new_internodes;
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
  spdlog::info("node {}, children {}, parent {}", node.id,
              node.children.size(), (void*)node.parent);
  
  if (node.axillary_bud) {
    Node* n = std::addressof(node);
    for (int i = 0; i < node.axillary_bud->new_internodes; i++) {
      glm::vec3 randv(getPointInSphereCap(node.direction, 60.0));
      glm::vec3 direction = randv;

      Node* new_node = new Node(n, n->position + direction*node.axillary_bud->internode_length, node.order + 1);
      new_node->axillary_bud = new Bud(n, marker_set, false);

      n->addChild(new_node);
      n = new_node;
    }

    n->terminal_bud = new Bud(n, marker_set, true);
  } else {
    for (auto &c : node.children) {
      if (c->order != node.order) {
        growShoots(*c);
      }
    }
  }

  if (node.terminal_bud) {
    Node* n = std::addressof(node);
    for (int i = 0; i < node.terminal_bud->new_internodes; i++) {
      glm::vec3 randv(getPointInSphereCap(node.direction, 60.0));
      glm::vec3 direction = direction = glm::normalize(node.direction) + glm::normalize(node.terminal_bud->optm_growth_direction) + randv + environment::tropism();

      Node* new_node = new Node(n, n->position + direction*node.terminal_bud->internode_length, node.order + 1);
      new_node->terminal_bud = new Bud(n, marker_set, false);

      n->addChild(new_node);
      n = new_node;
    }

    n->terminal_bud = new Bud(n, marker_set, true);
  } else {
    for (auto &c : node.children) {
      if (c->order == node.order) {
        growShoots(*c);
      }
    }
  }
  
  delete node.axillary_bud;
  delete node.terminal_bud;

  node.axillary_bud = nullptr;
  node.terminal_bud = nullptr;
}
