// main.c cube

#include "linalgebra.h"
#include "midnight.h"
#include "utils_gl.h"
#include <GLFW/glfw3.h>

Object* cube;
Camera *camera;
float time_delta;

void update() {
  rotate_x(cube->transform, 0.05f);
  rotate_y(cube->transform, 0.05f);
  rotate_z(cube->transform, 0.05f);
  draw_object(cube, camera);
}

int main(void) {
  
  MidnightWindow* window = midnight_init(400, 400);

  camera = create_camera(90.0f, 1, 0.1f, 100.0f);
  translate(camera->transform, 0.0f, 0.0f, -5.0f);
  
  Mesh* mesh = read_obj("cube.obj");
  Material* material = create_material("vertex.glsl", "fragment.glsl");
  cube = create_object(mesh, material);

  midnight_loop(window, update, &time_delta);

  free(camera);
  free(mesh);
  free(material);
  free(window);
  return 0;
}
