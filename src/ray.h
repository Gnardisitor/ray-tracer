#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef vec3 point3;
typedef struct {
    point3 origin;
    vec3 direction;
} ray;

void ray_create(ray *r, point3 *origin, vec3 *direction);
void ray_at(ray *r, double t, point3 *out);

#endif
