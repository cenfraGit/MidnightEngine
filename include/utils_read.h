// utils_read.h

#ifndef _UTILS_READ_H
#define _UTILS_READ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "glad/glad.h"

char* read_shader_source(const char* filename);
Mesh* read_obj(const char* filename);

#endif
