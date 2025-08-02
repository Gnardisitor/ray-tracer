#include "ray.h"

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
