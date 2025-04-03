// utils.h

#ifndef _UTILS_H
#define _UTILS_H

#include "types.h"

void mn_camera_move(MNCamera* camera, MNWindow* window, float* time_delta);
void mn_set_position(void* ptr, int type, float x, float y, float z);
void mn_init_transform(void* ptr, int type);

#endif
