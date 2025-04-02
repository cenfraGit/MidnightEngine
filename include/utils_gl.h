// utils_gl.h

#ifndef _UTILS_GL_H
#define _UTILS_GL_H

#include "glad/glad.h"
#include "types.h"

unsigned int* create_shader(const char* filename, int type);
unsigned int* create_program(const char* filename_vertex, const char* filename_fragment);

void set_uniform_matrix(const unsigned int program, const char *uniform_variable, float *matrix);
void draw_object(Object* object, Camera* camera);

Material* create_material(const char* filename_vertex, const char* filename_fragment);
Object* create_object(Mesh* mesh, Material* material);
Camera* create_camera(float fov_deg, float aspect_ratio, float near, float far);

#endif
