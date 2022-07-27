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
std::vector<vec3> shadows;

void getNodeVector(std::unique_ptr<Node>& node, int pindex) {
  Node* n = node.get();
  glm::vec3 pos = n->position;
  unsigned int this_index = static_cast<unsigned int>(nodes.size());

  if (pindex > -1) {
    indices.push_back(static_cast<unsigned int>(pindex));
    indices.push_back(this_index);
  }

  nodes.push_back(vec3(pos.x, pos.y, pos.z));
  spdlog::info("rendering vertex at ({},{},{})", pos.x, pos.y, pos.z);

  for (auto &c : n->children) {
    getNodeVector(c, static_cast<int>(this_index));
  }
}

void getShadowValues(Tree& tree) {
  spdlog::info("number of voxels: {}", tree.shadow_grid.grid.size());

  for (std::pair<std::string, environment::ShadowVoxel> element : tree.shadow_grid.grid) {
    if (element.second.value == 0.0f) {
      continue;
    }

    vector<string> tokens;
    stringstream check1(element.first);
    string intermediate;
    
    while(getline(check1, intermediate, ',')) {
      tokens.push_back(intermediate);
    }

    spdlog::info("rendering shadow at ({},{},{})", std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2]));

    shadows.push_back(vec3(std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2])));
  }
}

Window::Window() {}

int Window::run(int iterations) {
  Viewer viewer("Trees");
  MarkerSet m;
  Tree tree(m);
  tree.run(iterations);

  getNodeVector(tree.root, -1);
  getShadowValues(tree);

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

  auto shadow_points = new PointsDrawable("shadows");
  shadow_points->update_vertex_buffer(shadows);
  shadow_points->set_uniform_coloring(vec4(0.9f, 0.9f, 0.9f, 0.15f));  // r, g, b, a
  shadow_points->set_impostor_type(PointsDrawable::SPHERE);
  shadow_points->set_point_size(10);
  // viewer.add_drawable(shadow_points);

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
