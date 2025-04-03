// midnight.c

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "midnight.h"
#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------
// initialization
// ------------------------------------------------------------

void mn_init(void) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

// ------------------------------------------------------------
// callbacks
// ------------------------------------------------------------

void callback_framebuffer_size(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  MNCamera* camera = (MNCamera*)glfwGetWindowUserPointer(window);
  if (height > 0) { camera->aspect_ratio = (float)width/height; }
  else { camera->aspect_ratio = 1.0f; }
}

// ------------------------------------------------------------
// create window
// ------------------------------------------------------------

MNWindow* mn_create_window(const char* title, const int width, const int height) {

  /* --------- create glfw window --------- */

  GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window) { glfwTerminate(); perror("Failure initializing GLFW.\n"); }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, callback_framebuffer_size);

  /* ------------- glad setup ------------- */
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    perror("Failure initializing GLAD.\n");
  }

  /* ------------- gl states ------------- */

  glEnable(GL_DEPTH_TEST);
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
  
  return window;
}

// ------------------------------------------------------------
// main loop
// ------------------------------------------------------------

void mn_loop(MNWindow* window, void (*update)(void), float* time_delta) {

  float* time_delta_new;
  
  int initial_time_delta = 1;
  if (!time_delta) {
    initial_time_delta = 0;
    float* time_delta = (float*)malloc(sizeof(float));
    time_delta_new = time_delta;
  } else {
    time_delta_new = time_delta;
  }
  float last_frame;
  
  while (!glfwWindowShouldClose(window)) {

    // update time delta
    float current_frame = glfwGetTime();
    *time_delta_new = current_frame - last_frame;
    last_frame = current_frame;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    update();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // if time delta was malloced in this function, free
  if (!initial_time_delta) {
    free((void*)time_delta_new);
  }
  glfwTerminate();
}

// ------------------------------------------------------------
// create shader
// ------------------------------------------------------------

unsigned int* mn_create_shader(const char* filename, int type) {

  /* ------------- open file ------------- */

  FILE* file = fopen(filename, "r");
  if (!file) { prerror("Failure opening shader file.\n"); }

  /* -------------- get size -------------- */

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  /* ------- alloc mem for contents ------- */

  char* shader_source = (char*)malloc(file_size + 1);
  if (!shader_source) {
    fclose(file);
    prerror("Failure allocating memory for shader source code.\n");
  }

  /* ---------------- read ---------------- */

  size_t read_size = fread(shader_source, 1, file_size, file);
  if (read_size != (size_t)file_size) {
    free(shader_source);
    fclose(file);
    prerror("Failure reading shader source contents.\n");
  }
  shader_source[file_size] = '\0';
  fclose(file);

  /* ----------- create shader ----------- */
  // type=0 for GL_VERTEX_SHADER
  // type=1 for GL_FRAGMENT_SHADER

  unsigned int shader;
  if (type == 0) {
    shader = glCreateShader(GL_VERTEX_SHADER);
  } else if (type == 1) {
    shader = glCreateShader(GL_FRAGMENT_SHADER);
  } else {
    free(shader_source);
    prerror("Incorrect shader type.\n");
  }

  /* ----------- compile shader ----------- */

  glShaderSource(shader, 1, (const char**)&shader_source, NULL);
  glCompileShader(shader);

  int success;
  char info_log[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    glDeleteShader(shader);
    free(shader_source);
    fprintf(stderr, "Failure in shader compilation: %s\n%s\n", filename, info_log);
    return NULL;
  }
  free(shader_source);

  /* ------ alloc mem for ret value ------ */

  unsigned int* shader_id = (unsigned int*)malloc(sizeof(unsigned int));
  if (!shader_id) {
    glDeleteShader(shader);
    prerror("Failure in shader_id memory allocation.\n");
  }

  /* ---- set value and return pointer ---- */
  
  *shader_id = shader;
  return shader_id;
}

// ------------------------------------------------------------
// create program
// ------------------------------------------------------------

unsigned int* mn_create_program(const char* filename_vertex, const char* filename_fragment) {

  /* ---- create vert and frag shaders ---- */

  unsigned int* shader_vertex = mn_create_shader(filename_vertex, 0);
  if (!shader_vertex) { return NULL; }
  unsigned int* shader_fragment = mn_create_shader(filename_fragment, 1);
  if (!shader_fragment) { return NULL; }

  /* --------- create and compile --------- */

  unsigned int program = glCreateProgram();
  glAttachShader(program, *shader_vertex);
  glAttachShader(program, *shader_fragment);
  glLinkProgram(program);

  glDeleteShader(*shader_vertex);
  glDeleteShader(*shader_fragment);

  free(shader_vertex);
  free(shader_fragment);

  int success;
  char info_log[512];
  glGetShaderiv(program, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(program, 512, NULL, info_log);
    glDeleteProgram(program);
    fprintf(stderr, "Failure in shader program compilation: %s %s\n%s\n", filename_vertex, filename_fragment, info_log);
    return NULL;
  }

  /* ------ alloc mem for ret value ------ */

  unsigned int* program_id = (unsigned int*)malloc(sizeof(unsigned int));
  if (!program_id) {
    glDeleteProgram(program);
    perror("Failure in program_id memory allocation.\n");
  }

  /* ---- set value and return pointer ---- */

  *program_id = program;
  return program_id;
}

// ------------------------------------------------------------
// create mesh
// ------------------------------------------------------------

MNMesh* mn_create_mesh(const char* filename) {

  /* ------------- open file ------------- */
  
  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Failure opening obj file: %s\n", filename);
    return NULL;
  }

  /* -------- initialize mesh data -------- */

  unsigned int amount = 20000;
  Vertex* vertices = (Vertex*)malloc(amount * sizeof(Vertex));
  Face* faces = (Face*)malloc(amount * sizeof(Face));
  unsigned int num_vertices = 0;
  unsigned int num_faces = 0;

  /* ------------- parse file ------------- */

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

  /* --------- alloc mem for mesh --------- */

  MNMesh* mesh = (MNMesh*)malloc(sizeof(MNMesh));
  if (!mesh) { perror("Failure allocating memory for Mesh."); }

  /* ------------ set up data ------------ */

  mesh->num_vertices = num_faces * 3;
  mesh->vertices = (float*)malloc(mesh->num_vertices * 3 * sizeof(float));
  mesh->num_indices = num_faces * 3;
  mesh->indices = (unsigned int*)malloc(mesh->num_indices * sizeof(unsigned int));

  /* -- set flattened vertices and faces -- */

  for (unsigned int i = 0; i < num_faces; i++) {
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

  mesh->VAO = VAO;

  free(vertices);
  free(faces);
  
  return mesh;
}

// ------------------------------------------------------------
// create material
// ------------------------------------------------------------

MNMaterial* mn_create_material(const char* filename_vertex, const char* filename_fragment) {

  /* ----------- create program ----------- */
  
  unsigned int* program = mn_create_program(filename_vertex, filename_fragment);
  if (!program) { perror("Failure to create material program.\n"); }

  /* ------- alloc mem for material ------- */

  MNMaterial* material = (MNMaterial*)malloc(sizeof(MNMaterial));
  if (!material) { perror("Failure to allocate memory for material.\n"); }

  /* ------ set program and ret ptr ------ */
  
  material->program = *program;
  return material;
}

// ------------------------------------------------------------
// create object
// ------------------------------------------------------------

MNObject* mn_create_object(MNMesh* mesh, MNMaterial* material) {

  /* -------- alloc mem for object -------- */
  
  MNObject* object = (MNObject*)malloc(sizeof(MNObject));
  if (!object) { perror("Failure allocating memory for object.\n"); }
  
  object->mesh = mesh;
  object->material = material;
  mn_init_transform(object, 1);

  return object;
}

// ------------------------------------------------------------
// create camera
// ------------------------------------------------------------

MNCamera* mn_create_camera(float fov_deg, float aspect_ratio, float near, float far, MNWindow* window) {

  /* -------- alloc mem for camera -------- */
  
  MNCamera* camera = (MNCamera*)malloc(sizeof(MNCamera));
  if (!camera) { prerror("Failure allocating memory or Camera.\n"); }

  /* -------------- set data -------------- */
  
  camera->fov = radians(fov_deg);
  camera->aspect_ratio = aspect_ratio;
  camera->near = near;
  camera->far = far;
  mn_init_transform(camera, 0);

  // (needed to update aspect ratio inside window size callback)
  glfwSetWindowUserPointer(window, camera);
  
  return camera;
}

// ------------------------------------------------------------
// draw object
// ------------------------------------------------------------

void mn_draw_object(MNObject* object, MNCamera* camera) {

  glUseProgram(object->material->program);

  float* model = object->transform;
  float* view = camera->transform;
  float* projection = perspective(camera->fov,
				  camera->aspect_ratio,
				  camera->near,
				  camera->far);

  glUniformMatrix4fv(glGetUniformLocation(object->material->program, "model"), 1, GL_TRUE, model);
  glUniformMatrix4fv(glGetUniformLocation(object->material->program, "view"), 1, GL_TRUE, view);
  glUniformMatrix4fv(glGetUniformLocation(object->material->program, "projection"), 1, GL_TRUE, projection);
  
  free(projection);

  glBindVertexArray(object->mesh->VAO);
  glDrawElements(GL_TRIANGLES, object->mesh->num_indices, GL_UNSIGNED_INT, 0);
}

