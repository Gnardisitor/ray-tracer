#ifndef MAIN_H
#define MAIN_H

#include "hittable.h"
#include "camera.h"
#include "sphere.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"

#define PI                          3.1415926535897932385
#define DEG_TO_RAD(deg)             (deg * (PI / 180.0))

#define RAND_DOUBLE                 ((double)rand() / ((double)RAND_MAX + 1.0))
#define RAND_DOUBLE_RANGE(min, max) (min + (RAND_DOUBLE * (max - min)))

#endif
