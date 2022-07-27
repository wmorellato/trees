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
  double rads = (std::rand() % static_cast<int>(angle)) * M_PI / 180.0;
  glm::vec2 point_circle = glm::normalize(glm::circularRand<double>(1.0)) * sin(rads);
  glm::vec3 V = glm::vec3(point_circle.x, point_circle.y, cos(rads));
  return glm::quatLookAtLH(glm::normalize(direction), glm::vec3(0.0, 1.0, 0.0)) * V;
}

Tree::Tree(MarkerSet& _marker_set)
  : marker_set{_marker_set},
    shadow_grid{ShadowGrid(environment::voxel_size, glm::zero<glm::vec3>())} {
  createSeedling();
}

void Tree::createSeedling() {
  root = std::make_unique<Node>(nullptr, glm::vec3(0.0), 0);
  root->level = 1;
  root->terminal_bud = new Bud(root.get(), marker_set, true);
}

void Tree::run(int iterations) {
  for (int i = 0; i < iterations; i++) {
    runGrowthIteration(*root.get());
  }

  for (int x = -4; x < 5; x++) {
    for (int y = -4; y < 5; y++) {
      for (int z = -4; z < 5; z++) {
        glm::vec3 shadow_position(x, y, z);
        // spdlog::info("shadow at {0}: {1}", pv3(shadow_position), shadow_grid.getVoxelAtPosition(glm::vec3(x, y, z)).value);
      }
    } 
  }
}


void Tree::runGrowthIteration(Node& node) {
  // if (&node == nullptr) {
  //   spdlog::error("tree is null");
  //   return;
  // }

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
    node.terminal_bud->new_internodes = static_cast<int>(floorf(v));
    node.terminal_bud->internode_length = v / static_cast<float>(node.terminal_bud->new_internodes);
  }

  if (node.axillary_bud) {
    node.axillary_bud->new_internodes = static_cast<int>(floorf(v));
    node.axillary_bud->internode_length = v / static_cast<float>(node.axillary_bud->new_internodes);
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
              node.children.size(), static_cast<void*>(node.parent));
  
  if (node.axillary_bud) {
    Node* n = std::addressof(node);
    for (int i = 0; i < node.axillary_bud->new_internodes; i++) {
      glm::vec3 randv(getPointInSphereCap(node.direction, 60.0));
      glm::vec3 direction = randv;

      Node* new_node = new Node(n, n->position + direction*node.axillary_bud->internode_length, node.order + 1);
      new_node->axillary_bud = new Bud(n, marker_set, false);
      // casting shadow
      shadow_grid.propagateShadow(new_node->axillary_bud->position());

      n->addChild(new_node);
      n = new_node;
    }

    n->terminal_bud = new Bud(n, marker_set, true);
    // casting shadow
    shadow_grid.propagateShadow(n->terminal_bud->position());
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
      glm::vec3 direction = glm::normalize(node.direction) + glm::normalize(node.terminal_bud->optm_growth_direction) + randv + environment::tropism();
      // glm::vec3 direction = direction = glm::normalize(node.direction) + glm::normalize(node.terminal_bud->optm_growth_direction) + randv + environment::tropism();

      Node* new_node = new Node(n, n->position + direction*node.terminal_bud->internode_length, node.order + 1);
      new_node->terminal_bud = new Bud(n, marker_set, false);
      // casting shadow
      shadow_grid.propagateShadow(new_node->terminal_bud->position());

      n->addChild(new_node);
      n = new_node;
    }

    n->terminal_bud = new Bud(n, marker_set, true);
    // casting shadow
    shadow_grid.propagateShadow(n->terminal_bud->position());
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
