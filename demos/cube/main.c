// main.c cube

#include "midnight.h"
#include <stdlib.h>

MNObject* cube;
MNCamera* camera;
MNWindow* window;

float time_delta;
int width = 1200;
int height = 800;

void update() {
  rotate_x(cube->transform, 0.05f);
  rotate_y(cube->transform, 0.05f);
  rotate_z(cube->transform, 0.05f);
  mn_draw_object(cube, camera);
  mn_camera_move(camera, window, &time_delta);
}

int main(void) {

  mn_init();
  
  window = mn_create_window("Midnight", width, height);

  camera = mn_create_camera(90.0f, (float)width / height, 0.1f, 100.0f, window);
  translate(camera->transform, 0.0f, 0.0f, -5.0f);
  
  MNMesh* mesh = mn_create_mesh("cube.obj");
  MNMaterial* material = mn_create_material("vertex.glsl", "fragment.glsl");
  cube = mn_create_object(mesh, material);

  mn_loop(window, update, &time_delta);

  free(camera);
  free(mesh);
  free(material);
  return 0;
}
