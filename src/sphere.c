#include "sphere.h"

void sphere_create(sphere *s, point3 *center, double radius) {
    s->center[0] = (*center)[0];
    s->center[1] = (*center)[1];
    s->center[2] = (*center)[2];
    if (radius < 0.0) radius = 0.0;
    else s->radius = radius;
}

bool sphere_hit(sphere *s, double ray_tmin, double ray_tmax, ray *r, hit_record *rec) {
    // Compute discriminant
    vec3 oc;
    subtract(&s->center, &r->origin, &oc);
    double a = dot(&r->direction, &r->direction);
    double h = dot(&r->direction, &oc);
    double c = length_square(&oc) - (s->radius * s->radius);
    double discriminant = (h * h) - (a * c);

    // Check if intersection exists
    if (discriminant < 0) return false;

    // Find the nearest intersection point
    double sqrtd = sqrt(discriminant);
    double root = (h - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (h + sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) return false;
    }

    // Add hit to record
    rec->t = root;
    ray_at(r, rec->t, &rec->p);
    vec3 outward_normal;
    outward_normal[0] = (rec->p[0] - (s->center)[0]) / s->radius;
    outward_normal[1] = (rec->p[1] - (s->center)[1]) / s->radius;
    outward_normal[2] = (rec->p[2] - (s->center)[2]) / s->radius;
    set_face_normal(r, &outward_normal, rec);

    return true;
}
