#include "hittable.h"
#include "sphere.h"

/* Hit record definition*/

void set_face_normal(ray *r, vec3 *outward_normal, hit_record *rec) {
    // Assume outward_normal is a unit vector
    rec->front_face = dot(&r->direction, outward_normal) < 0;
    if (rec->front_face == false) negate(outward_normal, &rec->normal); // Flip normal if hitting from the back
    else create(&rec->normal, (*outward_normal)[0], (*outward_normal)[1], (*outward_normal)[2]);
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
    h->hit = (bool (*)(void *, double, double, ray *, hit_record *))sphere_hit;

    // Add hittable to list
    list->objects[list->count] = *h;
    list->count++;
}

bool hit(hittable_list *list, ray *r, double ray_tmin, double ray_tmax, hit_record *rec) {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_tmax;
    rec->t = -1.0;

    // Iterate through all hittables in the list
    for (int i = 0; i < list->count; i++) {
        hittable *obj = &list->objects[i];
        if (obj->hit(obj->object, ray_tmin, closest_so_far, r, &temp_rec) == true) {
            hit_anything = true;
            if (temp_rec.t < rec->t || rec->t < 0) *rec = temp_rec; // Update record with closest hit
        }
    }

    return hit_anything;
}
