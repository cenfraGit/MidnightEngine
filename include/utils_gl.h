// utils_gl.h

#ifndef _UTILS_GL_H
#define _UTILS_GL_H

#include "glad/glad.h"

static unsigned int* create_shader(const char* filename, int type);
unsigned int* create_program(const char* filename_vertex, const char* filename_fragment);

#endif
