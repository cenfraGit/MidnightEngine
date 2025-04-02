// midnight.c

#include "midnight.h"
#include <GLFW/glfw3.h>

MidnightWindow* midnight_init(const int width, const int height) {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(width, height, "Midnight", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failure initializing GLFW.\n");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failure initializing GLAD.\n");
    return NULL;
  }

  glEnable(GL_DEPTH_TEST);
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
  
  return window;
}

void midnight_loop(MidnightWindow* window, void (*update)(void), float* time_delta) {

  float* time_delta_new;
  
  int initial_time_delta = 1; // initially true
  if (!time_delta) {
    initial_time_delta = 0; // set to false
    float* time_delta = (float*)malloc(sizeof(float));
    time_delta_new = time_delta;
  } else {
    time_delta_new = time_delta;
  }
  float last_frame;
  
  while (!glfwWindowShouldClose(window)) {

    // update time delta
    float current_frame = glfwGetTime();
    *time_delta_new = current_frame - last_frame;
    last_frame = current_frame;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    update();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // if time delta was malloced in this function, free
  if (!initial_time_delta) {
    free((void*)time_delta_new);
  }

  glfwTerminate();  
}
