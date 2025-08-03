#include "vector.h"
#include "object.h"

/* VEC3 DEFINITION */

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

void random_vector(vec3 *a) {
    (*a)[0] = RAND_DOUBLE;
    (*a)[1] = RAND_DOUBLE;
    (*a)[2] = RAND_DOUBLE;
}

void random_range(vec3 *a, double min, double max) {
    (*a)[0] = RAND_DOUBLE_RANGE(min, max);
    (*a)[1] = RAND_DOUBLE_RANGE(min, max);
    (*a)[2] = RAND_DOUBLE_RANGE(min, max);
}

void random_unit_vector(vec3 *a) {
    vec3 p;
    while (true) {
        random_range(&p, -1.0, 1.0);
        double lensq = length_square(&p);
        if (1e-160 < lensq && lensq <= 1.0) {
            divide(&p, sqrt(lensq), a);
            return;
        }
    }
}

void random_on_hemisphere(vec3 *a, vec3 *normal) {
    vec3 on_unit_sphere;
    random_unit_vector(&on_unit_sphere);

    // Ensure the point is on the same hemisphere as the normal
    if (dot(&on_unit_sphere, normal) > 0.0) create(a, on_unit_sphere[0], on_unit_sphere[1], on_unit_sphere[2]);
    else negate(&on_unit_sphere, a);
}

bool near_zero(vec3 *a) {
    double s = 1e-8; // Small threshold
    return (fabs((*a)[0]) < s) && (fabs((*a)[1]) < s) && (fabs((*a)[2]) < s);
}

void reflect(vec3 *v, vec3 *n, vec3 *out) {
    double dot_product = dot(v, n);
    (*out)[0] = (*v)[0] - 2 * dot_product * (*n)[0];
    (*out)[1] = (*v)[1] - 2 * dot_product * (*n)[1];
    (*out)[2] = (*v)[2] - 2 * dot_product * (*n)[2];
}

void refract(vec3 *uv, vec3 *n, double etai_over_etat, vec3 *out) {
    // Find angle
    vec3 nuv;
    negate(uv, &nuv);
    double cos_theta = fmin(dot(&nuv, n), 1.0);

    // Calculate the perpendicular component
    vec3 r_out_perp;
    r_out_perp[0] = etai_over_etat * (((*uv)[0]) + (cos_theta * (*n)[0]));
    r_out_perp[1] = etai_over_etat * (((*uv)[1]) + (cos_theta * (*n)[1]));
    r_out_perp[2] = etai_over_etat * (((*uv)[2]) + (cos_theta * (*n)[2]));

    // Calculate the parallel component
    vec3 r_out_parallel;
    double len_sqrt = length_square(&r_out_perp);
    r_out_parallel[0] = -sqrt(fabs(1.0 - len_sqrt)) * (*n)[0];
    r_out_parallel[1] = -sqrt(fabs(1.0 - len_sqrt)) * (*n)[1];
    r_out_parallel[2] = -sqrt(fabs(1.0 - len_sqrt)) * (*n)[2];

    // Add the two components to get the refracted vector
    add(&r_out_perp, &r_out_parallel, out);
}

/* RAY DEFINITION */

void ray_create(ray *r, point3 *origin, vec3 *direction) {
    r->origin[0] = (*origin)[0];
    r->origin[1] = (*origin)[1];
    r->origin[2] = (*origin)[2];
    r->direction[0] = (*direction)[0];
    r->direction[1] = (*direction)[1];
    r->direction[2] = (*direction)[2];
}

void ray_at(ray *r, double t, point3 *out) {
    (*out)[0] = r->origin[0] + t * r->direction[0];
    (*out)[1] = r->origin[1] + t * r->direction[1];
    (*out)[2] = r->origin[2] + t * r->direction[2];
}

/* COLOR DEFINITION */

double linear_to_gamma(double linear_component) {
    if (linear_component > 0.0) return sqrt(linear_component);
    return 0.0; // Return 0 for negative or zero values
}

void write_color(FILE *image, color *color) {
    // Clamp color values to the range [0, 1] and convert to 8-bit integers
    interval intensity = {0.000, 0.999};
    int rbyte = (int)(256 * clamp(&intensity, linear_to_gamma((*color)[0])));
    int gbyte = (int)(256 * clamp(&intensity, linear_to_gamma((*color)[1])));
    int bbyte = (int)(256 * clamp(&intensity, linear_to_gamma((*color)[2])));
    fprintf(image, "%d %d %d\n", rbyte, gbyte, bbyte);
}
