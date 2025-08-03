#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "main.h"

/* VEC3 DEFINITION */

typedef double vec3[3];

void create(vec3 *a, double x, double y, double z);
void negate(vec3 *a, vec3 *out);
void add(vec3 *a, vec3 *b, vec3 *out);
void subtract(vec3 *a, vec3 *b, vec3 *out);
void multiply(vec3 *a, double t, vec3 *out);
void divide(vec3 *a, double t, vec3 *out);
void unit_vector(vec3 *a, vec3 *out);
void cross(vec3 *a, vec3 *b, vec3 *out);
double dot(vec3 *a, vec3 *b);
double length_square(vec3 *a);
double length(vec3 *a);
void random_vector(vec3 *a);
void random_range(vec3 *a, double min, double max);
void random_unit_vector(vec3 *a);
void random_on_hemisphere(vec3 *a, vec3 *normal);
bool near_zero(vec3 *a);
void reflect(vec3 *v, vec3 *n, vec3 *out);
void refract(vec3 *uv, vec3 *n, double etai_over_etat, vec3 *out);
void random_in_unit_disk(vec3 *a);

/* RAY DEFINITION */

typedef vec3 point3;
typedef struct {
    point3 origin;
    vec3 direction;
} ray;

void ray_create(ray *r, point3 *origin, vec3 *direction);
void ray_at(ray *r, double t, point3 *out);

/* COLOR DEFINITION */

typedef vec3 color;

double linear_to_gamma(double linear_component);
void write_color(FILE *image, color *color);

#endif
