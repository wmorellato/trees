#include "tree.hpp"
#include "window.hpp"
#include <iostream>

using namespace app;
using namespace tree;
using namespace easy3d;
using namespace environment;

std::vector<vec3> getBudVector(std::unique_ptr<Node> &node) {
  std::vector<vec3> buds;

  for (auto b : node->buds) {
    glm::vec3 gv = b.getPosition();
    buds.push_back(vec3(gv.x, gv.y, gv.z));
  }


  for (std::list<std::unique_ptr<Node>>::iterator it = node->children.begin(); it != node->children.end(); ++it) {
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

  Tree tree(m);
  std::unique_ptr<Node> node = std::make_unique<Node>(tree.root.get(), tree.root->position + glm::vec3(0.0, 1.0, 0.0), 1);
  node->addBud(Bud(node.get(), true));
  tree.root->addChild(node);

  std::vector<vec3> points;
  std::vector<vec3> buds = getBudVector(tree.root);

  for (std::list<Marker>::iterator it = tree.marker_set.markers.begin(); it != tree.marker_set.markers.end(); ++it) {
    std::cout << *it << std::endl;
    float x = it->position.x;
    float y = it->position.y;
    float z = it->position.z;

    points.insert(points.begin(), vec3(x, y, z));
  }

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
