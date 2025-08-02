#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

typedef struct sphere {
    point3 center;
    double radius;
} sphere;

void sphere_create(sphere *s, point3 *center, double radius);
bool sphere_hit(sphere *s, double ray_tmin, double ray_tmax, ray *r, hit_record *rec);

#endif
