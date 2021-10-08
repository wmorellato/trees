#include "window.hpp"
#include "tree.hpp"
#include "spdlog/spdlog.h"
#include <iostream>

using namespace app;
using namespace tree;
using namespace easy3d;
using namespace environment;

std::vector<unsigned int> indices;
std::vector<vec3> nodes;

void getNodeVector(std::unique_ptr<Node>& node, int pindex) {
  Node* n = node.get();
  glm::vec3 pos = n->position;
  int this_index = nodes.size();

  if (pindex > -1) {
    indices.push_back(pindex);
    indices.push_back(this_index);
  }

  nodes.push_back(vec3(pos.x, pos.y, pos.z));
  spdlog::info("pindex: {}, order: {}, this: {}", pindex, n->order, this_index);

  for (auto &c : n->children) {
    getNodeVector(c, this_index);
  }
}

Window::Window() {}

int Window::run(int iterations) {
  Viewer viewer("Trees");
  MarkerSet m;
  Tree tree(m);
  tree.run(iterations);

  getNodeVector(tree.root, -1);

  spdlog::info("qbase_max: {}", environment::qbase_max);
  spdlog::info("num_markers: {}", environment::num_markers);
  spdlog::info("r: {}", environment::r);
  spdlog::info("theta: {}", environment::theta);
  spdlog::info("ro: {}", environment::ro);
  spdlog::info("alpha: {}", environment::alpha);
  spdlog::info("lambda: {}", environment::lambda);

  auto vertices = new PointsDrawable("nodes");
  vertices->update_vertex_buffer(nodes);
  vertices->set_uniform_coloring(vec4(1.0f, 0.0f, 0.0f, 1.0f));  // r, g, b, a
  vertices->set_impostor_type(PointsDrawable::SPHERE);
  vertices->set_point_size(5);
  viewer.add_drawable(vertices);

  LinesDrawable* stems = new LinesDrawable("stems");
  stems->update_vertex_buffer(nodes);
  stems->update_element_buffer(indices);
  stems->set_uniform_coloring(vec4(0.0f, 0.0f, 1.0f, 1.0f));  // r, g, b, a
  stems->set_line_width(5.0);
  viewer.add_drawable(stems);


  (viewer.camera())->setUpVector(vec3(0.0, 1.0, 0.0));
  viewer.fit_screen();
  return viewer.run();
}
