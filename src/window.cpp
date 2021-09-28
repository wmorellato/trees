#include "window.hpp"
#include <iostream>

using namespace app;

Window::Window() : gl::demonstration("OBJ Demo", 960, 540), object(0), program(0) {
  if ((program = gl::init_program("../data/vertex.glsl", "../data/fragment.glsl"))) {
    glUseProgram(program);

    ProjectionMatrixLocation =
        glGetUniformLocation(program, "ProjectionMatrix");
    ModelViewMatrixLocation = glGetUniformLocation(program, "ModelViewMatrix");
    NormalMatrixLocation = glGetUniformLocation(program, "NormalMatrix");
    LightPositionLocation = glGetUniformLocation(program, "LightPosition");
    AmbientLightLocation = glGetUniformLocation(program, "AmbientLight");

    glUniform4f(AmbientLightLocation, 0.2, 0.2, 0.2, 1.0);

    if ((object = obj_create("../data/pokeball.obj"))) {
      obj_set_vert_loc(object, glGetAttribLocation(program, "vTangent"),
                       glGetAttribLocation(program, "vNormal"),
                       glGetAttribLocation(program, "vTexCoord"),
                       glGetAttribLocation(program, "vPosition"));

      obj_set_prop_loc(object, OBJ_KN, -1,
                       glGetUniformLocation(program, "NormalTexture"), -1);
      obj_set_prop_loc(object, OBJ_KD, -1,
                       glGetUniformLocation(program, "DiffuseTexture"), -1);
      obj_set_prop_loc(object, OBJ_KS, -1,
                       glGetUniformLocation(program, "SpecularTexture"), -1);
    }
  }

  glClearColor(0.2, 0.2, 0.2, 0.0);
  glEnable(GL_DEPTH_TEST);

  cam_position = gl::vec3(0, 0, 3);
}

void Window::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_TRUE, projection());
  glUniformMatrix4fv(ModelViewMatrixLocation,  1, GL_TRUE, view());
  glUniformMatrix3fv(NormalMatrixLocation,     1, GL_TRUE, normal(view()));
  glUniform4fv      (LightPositionLocation,    1,          light());

  if (object) obj_render(object);
}