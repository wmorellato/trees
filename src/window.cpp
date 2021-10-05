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

std::vector<vec3> getBudVector(std::unique_ptr<Node>& node) {
  std::vector<vec3> buds;

  for (auto b : node->buds) {
    glm::vec3 gv = b.position();
    buds.push_back(vec3(gv.x, gv.y, gv.z));
  }

  for (std::list<std::unique_ptr<Node>>::iterator it = node->children.begin();
       it != node->children.end(); ++it) {
    // I'm screwed if there are circular dependencies between nodes...
    std::vector<vec3> rv = getBudVector(*it);
    buds.insert(buds.begin(), rv.begin(), rv.end());
  }

  return buds;
}

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
  std::cout << "size in points " << nodes.size() << std::endl;

  auto vertices = new PointsDrawable("nodes");
  vertices->update_vertex_buffer(nodes);
  vertices->set_uniform_coloring(vec4(1.0f, 0.0f, 0.0f, 1.0f));  // r, g, b, a
  vertices->set_impostor_type(PointsDrawable::SPHERE);
  vertices->set_point_size(5);

  LinesDrawable* stems = new LinesDrawable("stems");
  stems->update_vertex_buffer(nodes);
  stems->update_element_buffer(indices);
  stems->set_uniform_coloring(vec4(0.0f, 0.0f, 1.0f, 1.0f));  // r, g, b, a
  stems->set_line_width(5.0);

  viewer.add_drawable(vertices);
  viewer.add_drawable(stems);

  (viewer.camera())->setUpVector(vec3(0.0, 1.0, 0.0));
  viewer.fit_screen();
  return viewer.run();
}
