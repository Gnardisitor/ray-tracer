#include "vec3.h"

void create(vec3 *a, double x, double y, double z) {
    (*a)[0] = x;
    (*a)[1] = y;
    (*a)[2] = z;
}

void negate(vec3 *a, vec3 *out) {
    (*out)[0] = -(*a)[0];
    (*out)[1] = -(*a)[1];
    (*out)[2] = -(*a)[2];
}

void add(vec3 *a, vec3 *b, vec3 *out) {
    (*out)[0] = (*a)[0] + (*b)[0];
    (*out)[1] = (*a)[1] + (*b)[1];
    (*out)[2] = (*a)[2] + (*b)[2];
}

void subtract(vec3 *a, vec3 *b, vec3 *out) {
    (*out)[0] = (*a)[0] - (*b)[0];
    (*out)[1] = (*a)[1] - (*b)[1];
    (*out)[2] = (*a)[2] - (*b)[2];
}

void multiply(vec3 *a, double t, vec3 *out) {
    (*out)[0] = t * (*a)[0];
    (*out)[1] = t * (*a)[1];
    (*out)[2] = t * (*a)[2];
}

void divide(vec3 *a, double t, vec3 *out) {
    (*out)[0] = (*a)[0] / t;
    (*out)[1] = (*a)[1] / t;
    (*out)[2] = (*a)[2] / t;
}

void cross(vec3 *a, vec3 *b, vec3 *out) {
    (*out)[0] = (*a)[1] * (*b)[2] - (*a)[2] * (*b)[1];
    (*out)[1] = (*a)[2] * (*b)[0] - (*a)[0] * (*b)[2];
    (*out)[2] = (*a)[0] * (*b)[1] - (*a)[1] * (*b)[0];
}

double dot(vec3 *a, vec3 *b) {
    return (*a)[0] * (*b)[0] + (*a)[1] * (*b)[1] + (*a)[2] * (*b)[2];
}

double length_square(vec3 *a) {
    return (*a)[0] * (*a)[0] + (*a)[1] * (*a)[1] + (*a)[2] * (*a)[2];
}

double length(vec3 *a) {
    return sqrt(length_square(a));
}

void unit_vector(vec3 *a, vec3 *out) {
    double len = length(a);
    if (len > 0.0) {
        (*out)[0] = (*a)[0] / len;
        (*out)[1] = (*a)[1] / len;
        (*out)[2] = (*a)[2] / len;
    } 
    else {
        (*out)[0] = 0.0;
        (*out)[1] = 0.0;
        (*out)[2] = 0.0;
    }
}
