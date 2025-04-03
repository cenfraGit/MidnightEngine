// midnight.h

#ifndef _MIDNIGHT_H
#define _MIDNIGHT_H

#include "types.h"
#include "linalgebra.h"
#include "utils.h"

void mn_init(void);
void mn_loop(MNWindow* window, void (*update)(void), float* time_delta);
void mn_draw_object(MNObject* object, MNCamera* camera);

MNWindow*     mn_create_window(const char* title, const int width, const int height);
unsigned int* mn_create_shader(const char* filename, int type);
unsigned int* mn_create_program(const char* filename_vertex, const char* filename_fragment);
MNMesh*       mn_create_mesh(const char* filename);
MNMaterial*   mn_create_material(const char* filename_vertex, const char* filename_fragment);
MNObject*     mn_create_object(MNMesh* mesh, MNMaterial* material);
MNCamera*     mn_create_camera(float fov_deg, float aspect_ratio, float near, float far, MNWindow* window);

#endif
