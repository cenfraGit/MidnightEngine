// main.c basic

#include "midnight.h"
#include "utils_gl.h"
#include <GLFW/glfw3.h>

int main(void) {
  
  MidnightWindow* window = midnight_init(400, 400);

  float time_delta, last_frame;

  unsigned int* program = create_program("vertex.glsl", "fragment.glsl");
  
  Mesh* mesh = read_obj("cube.obj");

  unsigned int VAO, VBO, EBO;
  if (mesh) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  }

  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    float current_frame = glfwGetTime();
    time_delta = current_frame - last_frame;
    last_frame = current_frame;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(*program);

    float* model = mat4(1.0f);
    float* view = mat4(1.0f);
    translate(view, 0, 0, -3);
    float* projection = perspective(radians(90.0f), 1, 0.1f, 100.0f);

    set_uniform_matrix(*program, "model", model);
    set_uniform_matrix(*program, "view", view);
    set_uniform_matrix(*program, "projection", projection);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->num_vertices * 3 * sizeof(float), mesh->vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->num_indices * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
