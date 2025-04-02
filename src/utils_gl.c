// utils_gl.c

#include <stdio.h>
#include "utils_gl.h"
#include "utils_read.h"

// ------------------------------------------------------------
// create shader
// ------------------------------------------------------------

unsigned int* create_shader(const char* filename, int type) {
  
  const char* source = read_shader_source(filename);
  if (!source) {
    fprintf(stderr, "Failed to read shader source %s\n", filename);
    return NULL;
  }

  unsigned int shader;
  if (type == 0) {
    shader = glCreateShader(GL_VERTEX_SHADER);
  } else if (type == 1) {
    shader = glCreateShader(GL_FRAGMENT_SHADER);
  } else {
    fprintf(stderr, "Incorrect shader type.");
    free((void*)source);
    return NULL;
  }

  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success;
  char info_log[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    fprintf(stderr, "Failure in shader compilation: %s\n%s\n", filename, info_log);
    glDeleteShader(shader);
    free((void*)source);
    return NULL;
  }

  free((void*)source);

  unsigned int* shader_id = (unsigned int*)malloc(sizeof(unsigned int));
  if (!shader_id) {
    fprintf(stderr, "Failed to allocate memory for return shader id.\n");
    glDeleteShader(shader);
    return NULL;
  }

  *shader_id = shader;
  return shader_id;
}

// ------------------------------------------------------------
// create program
// ------------------------------------------------------------

unsigned int* create_program(const char* filename_vertex, const char* filename_fragment) {

  unsigned int* shader_vertex = create_shader(filename_vertex, 0);
  if (!shader_vertex) {
    return NULL;
  }
  unsigned int* shader_fragment = create_shader(filename_fragment, 1);
  if (!shader_fragment) {
    return NULL;
  }

  unsigned int program = glCreateProgram();
  glAttachShader(program, *shader_vertex);
  glAttachShader(program, *shader_fragment);
  glLinkProgram(program);

  glDeleteShader(*shader_vertex);
  glDeleteShader(*shader_fragment);

  free((void*)shader_vertex);
  free((void*)shader_fragment);

  int success;
  char info_log[512];
  glGetShaderiv(program, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(program, 512, NULL, info_log);
    fprintf(stderr, "Failure in shader program compilation: %s %s\n%s\n", filename_vertex, filename_fragment, info_log);
    glDeleteProgram(program);
    return NULL;
  }

  unsigned int* program_id = (unsigned int*)malloc(sizeof(unsigned int));
  if (!program_id) {
    fprintf(stderr, "Failed to allocate memory for return program id.\n");
    glDeleteProgram(program);
    return NULL;
  }

  *program_id = program;
  return program_id;
}

// ------------------------------------------------------------
// uniforms
// ------------------------------------------------------------

void set_uniform_matrix(const unsigned int program, const char *uniform_variable, float *matrix) {
  glUniformMatrix4fv(glGetUniformLocation(program, uniform_variable), 1, GL_TRUE, matrix);
}
