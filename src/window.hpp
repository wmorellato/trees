#include "GLFundamentals.hpp"
#include "GLDemonstration.hpp"
#include <obj/obj.h>
// #include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace app {

class Window : public gl::demonstration {
 private:
  obj* object;
  GLuint program;

  GLint ProjectionMatrixLocation;
  GLint ModelViewMatrixLocation;
  GLint NormalMatrixLocation;
  GLint LightPositionLocation;
  GLint AmbientLightLocation;

 public:
  Window();
  void draw();
};

}  // namespace app
