#ifndef VEC3_H
#define VEC3_H

#include <math.h>

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

#endif
