// utils.c

#include "utils.h"
#include "linalgebra.h"
#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------
// move camera
// ------------------------------------------------------------

void mn_camera_move(MNCamera* camera, MNWindow* window, float* time_delta) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
  float amount_movement = 3.0f * *time_delta;
  float amount_rotation = 100.0f * *time_delta;
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    translate(camera->transform, 0.0f, 0.0f, amount_movement);
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    translate(camera->transform, amount_movement, 0.0f, 0.0f);
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    translate(camera->transform, 0.0f, 0.0f, -amount_movement);
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    translate(camera->transform, -amount_movement, 0.0f, 0.0f);
  if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    translate(camera->transform, 0.0f, -amount_movement, 0.0f);
  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    translate(camera->transform, 0.0f, amount_movement, 0.0f);
  if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    rotate_y(camera->transform, amount_rotation);
}

// ------------------------------------------------------------
// set position
// ------------------------------------------------------------

void mn_set_position(void* ptr, int type, float x, float y, float z) {
  if (type == 0) {
    MNCamera* camera = (MNCamera*)ptr;
    camera->transform[3]  = x;
    camera->transform[7]  = y;
    camera->transform[11] = z;
  }
  else if (type == 1) {
    MNObject* object = (MNObject*)ptr;
    object->transform[3]  = x;
    object->transform[7]  = y;
    object->transform[11] = z;
  }
}

// ------------------------------------------------------------
// init transform
// ------------------------------------------------------------

void mn_init_transform(void* ptr, int type) {
  float* identity = mat4(1.0f);
  if (type == 0) {
    MNCamera* camera = (MNCamera*)ptr;
    for (int i = 0; i < 16; i++) {
      camera->transform[i] = identity[i];
    }
  }
  else if (type == 1) {
    MNObject* object = (MNObject*)ptr;
    for (int i = 0; i < 16; i++) {
      object->transform[i] = identity[i];
    }
  }
  free(identity);
}
