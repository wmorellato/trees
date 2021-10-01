#include "window.hpp"
#include "tree.hpp"
#include <iostream>

using namespace app;
using namespace tree;
using namespace easy3d;
using namespace environment;

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

std::vector<vec3> getNodeVector(std::unique_ptr<Node>& node) {
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

Window::Window() {}

int Window::run() {
  // const std::vector<vec3> &points = resource::bunny_vertices;
  MarkerSet m;
  glm::vec3 c(2.f, 2.f, 2.f);

  Viewer viewer("Test");

  std::vector<vec3> points;
  for (auto marker : m.markers) {
    points.push_back(
        vec3(marker.position.x, marker.position.y, marker.position.z));
  }

  Tree tree(m);
  tree.run(5);

  std::vector<vec3> buds = getBudVector(tree.root);

  std::cout << "size in points " << points.size() << std::endl;

  auto vertices = new PointsDrawable("faces");
  // Upload the vertex positions of the surface to the GPU.
  vertices->update_vertex_buffer(points);
  // Draw the vertices in red.
  vertices->set_uniform_coloring(vec4(1.0f, 0.0f, 0.0f, 1.0f));  // r, g, b, a
  // Draw the vertices as spheres.
  vertices->set_impostor_type(PointsDrawable::SPHERE);
  // Set the vertices size to 10 pixels.
  vertices->set_point_size(5);

  auto vertices2 = new PointsDrawable("faces");
  // Upload the vertex positions of the surface to the GPU.
  vertices2->update_vertex_buffer(buds);
  // Draw the vertices in red.
  vertices2->set_uniform_coloring(vec4(0.0f, 1.0f, 0.0f, 1.0f));  // r, g, b, a
  // Draw the vertices as spheres.
  vertices2->set_impostor_type(PointsDrawable::SPHERE);
  // Set the vertices size to 10 pixels.
  vertices2->set_point_size(10);

  // Add the drawable to the viewer
  viewer.add_drawable(vertices);
  viewer.add_drawable(vertices2);

  viewer.fit_screen();

  return viewer.run();
}
