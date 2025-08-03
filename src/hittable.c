#include "hittable.h"
#include "sphere.h"

/* Hit record definition*/

void set_face_normal(ray *r, vec3 *outward_normal, hit_record *rec) {
    // Assume outward_normal is a unit vector
    rec->front_face = dot(&r->direction, outward_normal) < 0;
    if (rec->front_face == false) negate(outward_normal, &rec->normal); // Flip normal if hitting from the back
    else create(&rec->normal, (*outward_normal)[0], (*outward_normal)[1], (*outward_normal)[2]);
}

/* Interval definition */

double size(interval *i) {
    return i->tmax - i->tmin;
}

bool contains(interval *i, double x) {
    return i->tmin <= x && x <= i->tmax;
}

bool surrounds(interval *i, double x) {
    return i->tmin < x && x < i->tmax;
}

double clamp(interval *i, double x) {
    if (x < i->tmin) return i->tmin;
    if (x > i->tmax) return i->tmax;
    return x;
}

/* Hittable object list definition */

void add_sphere(hittable_list *list, double x, double y, double z, double radius) {
    // Check if list is full
    if (list->count >= 128) {
        fprintf(stderr, "Hittable list is full\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for pointer to hittable object
    hittable *h = malloc(sizeof(hittable));
    sphere *s = malloc(sizeof(sphere));
    if (h == NULL || s == NULL) {
        fprintf(stderr, "Memory allocation failed for sphere\n");
        exit(EXIT_FAILURE);
    }

    // Set object and hit function
    point3 center = {x, y, z};
    sphere_create(s, &center, radius);
    h->object = s;
    h->hit = (bool (*)(void *, interval *, ray *, hit_record *))sphere_hit;

    // Add hittable to list
    list->objects[list->count] = *h;
    list->count++;
}

bool hit(hittable_list *list, ray *r, interval *ray_t, hit_record *rec) {
    hit_record temp_rec;
    interval current_t;
    bool hit_anything = false;
    double closest_so_far = ray_t->tmax;

    // Iterate through all hittables in the list
    for (int i = 0; i < list->count; i++) {
        hittable *obj = &list->objects[i];

        // Create interval for current hittable
        current_t.tmin = ray_t->tmin;
        current_t.tmax = closest_so_far;

        // Check if hit
        if (obj->hit(obj->object, &current_t, r, &temp_rec) == true) {
            // Update closest hit
            hit_anything = true;
            closest_so_far = temp_rec.t;

            // Update closest hit record
            rec->t = temp_rec.t;
            rec->p[0] = temp_rec.p[0];
            rec->p[1] = temp_rec.p[1];
            rec->p[2] = temp_rec.p[2];
            rec->normal[0] = temp_rec.normal[0];
            rec->normal[1] = temp_rec.normal[1];
            rec->normal[2] = temp_rec.normal[2];
            rec->front_face = temp_rec.front_face;
        }
    }

    return hit_anything;
}
