// midnight.c

#include "midnight.h"
#include <GLFW/glfw3.h>

GLFWwindow* midnight_init(const int width, const int height) {

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
  
  return window;
}
