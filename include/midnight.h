// midnight.h

#ifndef _MIDNIGHT_H
#define _MIDNIGHT_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "linalgebra.h"
#include "utils_read.h"

GLFWwindow* midnight_init(const int width, const int height);

#endif
