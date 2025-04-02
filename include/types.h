// types.h

#ifndef _TYPES_H
#define _TYPES_H

typedef struct { float x, y, z; } Vertex;
typedef struct { unsigned int v1, v2, v3; } Face;
typedef struct {
  float* vertices;
  unsigned int num_vertices;
  unsigned int* indices;
  unsigned int num_indices;
} Mesh;

typedef struct {
  unsigned int program;
} Material;

typedef struct {
  Mesh* mesh;
  Material* material;
  unsigned int VAO;
  float transform[16];
} Object;

typedef struct {
  float fov;
  float aspect_ratio;
  float near;
  float far;
  float transform[16];
} Camera;

#endif
