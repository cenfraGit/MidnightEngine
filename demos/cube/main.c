// main.c cube

#include "midnight.h"
#include <stdlib.h>
#include <stdio.h>

MNObject* cube;
MNCamera* camera;
MNWindow* window;

int width = 1200;
int height = 800;

float velocity = 0.0;
float gravity = 3;
float time_delta = 0.0f;

void update() {
  velocity = velocity + (gravity * time_delta);
  translate(cube->transform, 0.0f, -(velocity * time_delta), 0.0f);
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
