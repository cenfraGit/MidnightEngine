// types.h

#ifndef _TYPES_H
#define _TYPES_H

#include <GLFW/glfw3.h>

#define prerror(value) do{ fprintf(stderr, value); return NULL; } while (0);

typedef GLFWwindow MNWindow;

typedef struct { float x, y, z; } Vertex;
typedef struct { unsigned int v1, v2, v3; } Face;
typedef struct {
  float* vertices;
  unsigned int num_vertices;
  unsigned int* indices;
  unsigned int num_indices;
  unsigned int VAO;
} MNMesh;

typedef struct {
  unsigned int program;
} MNMaterial;

typedef struct {
  MNMesh* mesh;
  MNMaterial* material;
  /* unsigned int VAO; */
  float transform[16];
} MNObject;

typedef struct {
  float fov;
  float aspect_ratio;
  float near;
  float far;
  float transform[16];
} MNCamera;

#endif
