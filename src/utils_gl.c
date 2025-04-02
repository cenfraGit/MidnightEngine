// utils_gl.c

#include <stdio.h>
#include "linalgebra.h"
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

// ------------------------------------------------------------
// create material
// ------------------------------------------------------------

Material* create_material(const char* filename_vertex, const char* filename_fragment) {
  unsigned int* program = create_program(filename_vertex, filename_fragment);
  if (!program) {
    fprintf(stderr, "Failure to create material program.\n");
    return NULL;
  }

  Material* material = (Material*)malloc(sizeof(Material));
  if (!material) {
    fprintf(stderr, "Failure to allocate memory for material.\n");
    return NULL;
  }
  material->program = *program;
  return material;
}

// ------------------------------------------------------------
// create object
// ------------------------------------------------------------

Object* create_object(Mesh* mesh, Material* material) {
  
  Object* object = (Object*)malloc(sizeof(Object));
  if (!object) {
    fprintf(stderr, "Failure allocating memory for object.\n");
    return NULL;
  }
  
  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, mesh->num_vertices * 3 * sizeof(float), mesh->vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->num_indices * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  object->mesh = mesh;
  object->material = material;
  object->VAO = VAO;
  init_transform(object, 1);

  return object;
}

// ------------------------------------------------------------
// create camera
// ------------------------------------------------------------

Camera* create_camera(float fov_deg, float aspect_ratio, float near, float far) {
  Camera* camera = (Camera*)malloc(sizeof(Camera));
  if (!camera) {
    fprintf(stderr, "Failure allocating memory or Camera.\n");
    return NULL;
  }
  camera->fov = radians(fov_deg);
  camera->aspect_ratio = aspect_ratio;
  camera->near = near;
  camera->far = far;
  init_transform(camera, 0);

  return camera;
}

// ------------------------------------------------------------
// draw object
// ------------------------------------------------------------

void draw_object(Object* object, Camera* camera) {

  glUseProgram(object->material->program);

  float* model = object->transform;
  float* view = camera->transform;
  float* projection = perspective(camera->fov,
				  camera->aspect_ratio,
				  camera->near,
				  camera->far);

  set_uniform_matrix(object->material->program, "model", model);
  set_uniform_matrix(object->material->program, "view", view);
  set_uniform_matrix(object->material->program, "projection", projection);
  
  free(projection);

  glBindVertexArray(object->VAO);
  glDrawElements(GL_TRIANGLES, object->mesh->num_indices, GL_UNSIGNED_INT, 0);
}
