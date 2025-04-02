// linalgebra.h

#ifndef _LINALGEBRA_H
#define _LINALGEBRA_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define 3.141592657
#endif

#define radians(value) ((value) * M_PI / 180.0f)

float* mat4(float n);
void mat4_print(float* matrix);
void mat4_multiply(float* M1, float* M2); // applied inplace to M1

float* perspective(float fov, float aspect_ratio, float near, float far);

void scale(float* matrix, float s);
void translate(float* matrix, float tx, float ty, float tz);
void rotate_x(float* matrix, float angle);
void rotate_y(float* matrix, float angle);
void rotate_z(float* matrix, float angle);

void set_position(void* ptr, int type, float x, float y, float z);
void init_transform(void* ptr, int type);

#endif
