#ifndef HITTABLE_H
#define HITTABLE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "ray.h"

/* Hit record definition*/

typedef struct {
    bool front_face;
    vec3 normal;
    point3 p;
    double t;
} hit_record;

void set_face_normal(ray *r, vec3 *outward_normal, hit_record *rec);

/* Interval definition */

typedef struct {
    double tmin;
    double tmax;
} interval;

double size(interval *i);
bool contains(interval *i, double x);
bool surrounds(interval *i, double x);

/* Hittable object definition */

typedef struct {
    void *object;
    bool (*hit)(void *object, interval *i, ray *r, hit_record *rec);
} hittable;

/* Hittable object list definition */

typedef struct {
    hittable objects[128];
    int count;
} hittable_list;

void add_sphere(hittable_list *list, double x, double y, double z, double radius);
bool hit(hittable_list *list, ray *r, interval *ray_t, hit_record *rec);

#endif
