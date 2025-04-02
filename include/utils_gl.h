// utils_gl.h

#ifndef _UTILS_GL_H
#define _UTILS_GL_H

#include "glad/glad.h"

static unsigned int* create_shader(const char* filename, int type);
unsigned int* create_program(const char* filename_vertex, const char* filename_fragment);

void set_uniform_matrix(const unsigned int program, const char *uniform_variable, float *matrix);

#endif
