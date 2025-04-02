// engine.c

#include "engine.h"
#include <GLFW/glfw3.h>

int midnight_init(void) {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(400, 400, "Midnight", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failure initializing GLFW.\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failure initializing GLAD.\n");
    return -1;
  }

  while (!glfwWindowShouldClose(window)) {

  }
  
  return 0;
  
}
