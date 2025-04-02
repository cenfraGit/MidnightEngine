// utils_read.c

#include "utils_read.h"

// ------------------------------------------------------------
// read shader
// ------------------------------------------------------------

char* read_shader_source(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Failure opening shader source file.\n");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* shader_source = (char*)malloc(file_size + 1);
  if (!shader_source) {
    fprintf(stderr, "Failure allocating memory for shader source.\n");
    fclose(file);
    return NULL;
  }

  size_t read_size = fread(shader_source, 1, file_size, file);
  if (read_size != (size_t)file_size) {
    fprintf(stderr, "Failure reading shader source file.\n");
    free(shader_source);
    fclose(file);
    return NULL;
  }

  shader_source[file_size] = '\0';
  fclose(file);

  return shader_source;
}

// ------------------------------------------------------------
// read obj
// ------------------------------------------------------------

Mesh* read_obj(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Failure opening obj file.\n");
    return NULL;
  }

  unsigned int amount = 5120;
  Vertex* vertices = (Vertex*)malloc(amount * sizeof(Vertex));
  Face* faces = (Face*)malloc(amount * sizeof(Face));
  unsigned int num_vertices = 0;
  unsigned int num_faces = 0;

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "v ", 2) == 0) {
      Vertex v;
      sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
      vertices[num_vertices++] = v;
    } else if (strncmp(line, "f ", 2) == 0) {
      Face f;
      sscanf(line, "f %u %u %u", &f.v1, &f.v2, &f.v3);
      f.v1--; f.v2--; f.v3--; // adjust
      faces[num_faces++] = f;
    }
  }
  fclose(file);

  /* --------- save data to mesh --------- */

  Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
  if (!mesh) {
    fprintf(stderr, "Failure allocating memory for Mesh.\n");
    fclose(file);
    return NULL;
  }

  mesh->num_vertices = num_faces * 3;
  mesh->vertices = (float*)malloc(mesh->num_vertices * 3 * sizeof(float));
  mesh->num_indices = num_faces * 3;
  mesh->indices = (unsigned int*)malloc(mesh->num_indices * sizeof(unsigned int));

  for (unsigned int i = 0; i < num_faces; ++i) {
    Face face = faces[i];
    Vertex v1 = vertices[face.v1];
    Vertex v2 = vertices[face.v2];
    Vertex v3 = vertices[face.v3];

    mesh->vertices[i * 9] = v1.x;
    mesh->vertices[i * 9 + 1] = v1.y;
    mesh->vertices[i * 9 + 2] = v1.z;

    mesh->vertices[i * 9 + 3] = v2.x;
    mesh->vertices[i * 9 + 4] = v2.y;
    mesh->vertices[i * 9 + 5] = v2.z;

    mesh->vertices[i * 9 + 6] = v3.x;
    mesh->vertices[i * 9 + 7] = v3.y;
    mesh->vertices[i * 9 + 8] = v3.z;

    mesh->indices[i * 3] = i * 3;
    mesh->indices[i * 3 + 1] = i * 3 + 1;
    mesh->indices[i * 3 + 2] = i * 3 + 2;
  }

  free(vertices);
  free(faces);
  
  return mesh;
}
