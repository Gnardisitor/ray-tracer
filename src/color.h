#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>

#include "hittable.h"
#include "vec3.h"

typedef vec3 color;

void write_color(FILE *image, color *color);

#endif
