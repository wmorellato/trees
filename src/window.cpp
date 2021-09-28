#include "markers.hpp"
#include "window.hpp"
#include <iostream>

using namespace app;
using namespace easy3d;
using namespace environment;

Window::Window() {}

int Window::run() {
  // const std::vector<vec3> &points = resource::bunny_vertices;
  MarkerSet m;
  glm::vec3 c(2.f, 2.f, 2.f);
  m.AddToSphere(c, 2.0, 1000);

  Viewer viewer("Test");

  std::vector<vec3> points;
  for (std::vector<Marker>::iterator it = std::begin(m.markers); it != std::end(m.markers); ++it) {
    float x = it->point.x;
    float y = it->point.y;
    float z = it->point.z;

    points.insert(points.begin(), vec3(x, y, z));
  }

  auto vertices = new PointsDrawable("faces");
  // Upload the vertex positions of the surface to the GPU.
  vertices->update_vertex_buffer(points);
  
  // Draw the vertices in red.
  vertices->set_uniform_coloring(vec4(1.0f, 0.0f, 0.0f, 1.0f));  // r, g, b, a
  // Draw the vertices as spheres.
  vertices->set_impostor_type(PointsDrawable::SPHERE);
  // Set the vertices size to 10 pixels.
  vertices->set_point_size(5);
  // Add the drawable to the viewer
  viewer.add_drawable(vertices);

  viewer.fit_screen();

  return viewer.run();
}
