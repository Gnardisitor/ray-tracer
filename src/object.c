#include "object.h"

/* HIT RECORD DEFINITION */

void set_face_normal(ray *r, vec3 *outward_normal, hit_record *rec) {
    // Assume outward_normal is a unit vector
    rec->front_face = dot(&r->direction, outward_normal) < 0;
    if (rec->front_face == false) negate(outward_normal, &rec->normal); // Flip normal if hitting from the back
    else create(&rec->normal, (*outward_normal)[0], (*outward_normal)[1], (*outward_normal)[2]);
}

/* INTERVAL DEFINITION */

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

/* MATERIAL DEFINITION */

/* LAMBERTIAN MATERIAL DEFINITION */

void create_lambertian(material *mat, color *albedo) {
    mat->type = LAMBERTIAN;
    mat->data = malloc(sizeof(lambertian_data));
    if (mat->data == NULL) {
        fprintf(stderr, "Memory allocation failed for lambertian data\n");
        exit(EXIT_FAILURE);
    }
    create(&(((lambertian_data *)mat->data)->albedo), (*albedo)[0], (*albedo)[1], (*albedo)[2]);
    mat->scatter = lambertian_scatter;
}

bool lambertian_scatter(ray *r, hit_record *rec, color *attenuation, ray *scattered) {
    // Uselss check to compile (r not used since random scatter direction)
    if (r == NULL) exit(EXIT_FAILURE);

    // Find scatter direction
    vec3 scatter_direction, unit;
    random_unit_vector(&unit);
    add(&rec->normal, &unit, &scatter_direction);

    // Catch degenerate scatter direction
    if (near_zero(&scatter_direction)) {
        create(&scatter_direction, rec->normal[0], rec->normal[1], rec->normal[2]);
    }

    // Create scattered ray
    create(attenuation, ((lambertian_data *)rec->mat->data)->albedo[0], ((lambertian_data *)rec->mat->data)->albedo[1], ((lambertian_data *)rec->mat->data)->albedo[2]);
    create(&scattered->origin, rec->p[0], rec->p[1], rec->p[2]);
    create(&scattered->direction, scatter_direction[0], scatter_direction[1], scatter_direction[2]);

    return true; // Always scatter for lambertian material
}

/* METAL MATERIAL DEFINITION */

void create_metal(material *mat, color *albedo, double fuzz) {
    mat->type = METAL;
    mat->data = malloc(sizeof(metal_data));
    if (mat->data == NULL) {
        fprintf(stderr, "Memory allocation failed for metal data\n");
        exit(EXIT_FAILURE);
    }
    create(&(((metal_data *)mat->data)->albedo), (*albedo)[0], (*albedo)[1], (*albedo)[2]);
    ((metal_data *)mat->data)->fuzz = fuzz;
    mat->scatter = metal_scatter;
}

bool metal_scatter(ray *r, hit_record *rec, color *attenuation, ray *scattered) {
    // Find reflected direction
    vec3 reflected, unit, fuzzed;
    reflect(&r->direction, &rec->normal, &reflected);

    // Add fuzz to the reflected direction
    random_unit_vector(&unit);
    multiply(&unit, ((metal_data *)rec->mat->data)->fuzz, &fuzzed);
    unit_vector(&reflected, &unit);
    add(&reflected, &fuzzed, &reflected);

    // Create scattered ray
    create(attenuation, ((metal_data *)rec->mat->data)->albedo[0], ((metal_data *)rec->mat->data)->albedo[1], ((metal_data *)rec->mat->data)->albedo[2]);
    create(&scattered->origin, rec->p[0], rec->p[1], rec->p[2]);
    create(&scattered->direction, reflected[0], reflected[1], reflected[2]);

    return true; // Always scatter for metal material
}

/* DIELECTRIC MATERIAL DEFINITION */

void create_dielectric(material *mat, double refraction_index) {
    mat->type = DIELECTRIC;
    mat->data = malloc(sizeof(dielectric_data));
    if (mat->data == NULL) {
        fprintf(stderr, "Memory allocation failed for dielectric data\n");
        exit(EXIT_FAILURE);
    }
    ((dielectric_data *)mat->data)->refraction_index = refraction_index;
    mat->scatter = dielectric_scatter;
}

bool dielectric_scatter(ray *r, hit_record *rec, color *attenuation, ray *scattered) {
    // Attenuation is always white for dielectric
    create(attenuation, 1.0, 1.0, 1.0);

    // Calculate refraction indices
    double ri = ((dielectric_data *)rec->mat->data)->refraction_index;
    if (rec->front_face == true) ri = 1.0 / ri;

    // Check for total internal reflection
    vec3 unit_direction, nunit_direction, direction;
    unit_vector(&r->direction, &unit_direction);
    negate(&unit_direction, &nunit_direction);
    double cos_theta = fmin(dot(&nunit_direction, &rec->normal), 1.0);
    double sin_theta = sqrt(1.0 - (cos_theta * cos_theta));

    // Check if total internal reflection occurs
    if (ri * sin_theta > 1.0 || reflectance(cos_theta, ri) > RAND_DOUBLE) {
        reflect(&unit_direction, &rec->normal, &direction);
    } 
    else {
        refract(&unit_direction, &rec->normal, ri, &direction);
    }

    // Create scattered ray
    ray_create(scattered, &rec->p, &direction);

    return true; // Always scatter for dielectric material
}

double reflectance(double cosine, double refraction_index) {
    // Use Schlick's approximation for reflectance
    double r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5);
}

/* SPHERE DEFINITION */

void sphere_create(sphere *s, point3 *center, double radius, material *mat) {
    s->mat = mat;
    s->center[0] = (*center)[0];
    s->center[1] = (*center)[1];
    s->center[2] = (*center)[2];
    if (radius < 0.0) radius = 0.0;
    else s->radius = radius;
}

bool sphere_hit(sphere *s, interval *ray_t, ray *r, hit_record *rec) {
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
    if (surrounds(ray_t, root) == false) {
        root = (h + sqrtd) / a;
        if (surrounds(ray_t, root) == false) return false;
    }

    // Add hit to record
    rec->t = root;
    ray_at(r, rec->t, &rec->p);
    vec3 outward_normal;
    outward_normal[0] = (rec->p[0] - (s->center)[0]) / s->radius;
    outward_normal[1] = (rec->p[1] - (s->center)[1]) / s->radius;
    outward_normal[2] = (rec->p[2] - (s->center)[2]) / s->radius;
    set_face_normal(r, &outward_normal, rec);
    rec->mat = s->mat;

    return true;
}

/* OBJECT LIST DEFINITION */

void add_sphere(hittable_list *list, double x, double y, double z, double radius, material *mat) {
    // Check if list is full
    if (list->count >= 500) {
        fprintf(stderr, "Hittable list is full\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for pointer to hittable object
    sphere *s = malloc(sizeof(sphere));
    if (s == NULL) {
        fprintf(stderr, "Memory allocation failed for sphere\n");
        exit(EXIT_FAILURE);
    }

    // Set object and hit function
    point3 center = {x, y, z};
    sphere_create(s, &center, radius, mat);

    // Create hittable structure
    hittable h = {
        .object = s,
        .hit = (bool (*)(void *, interval *, ray *, hit_record *))sphere_hit
    };

    // Add hittable to list
    list->objects[list->count] = h;
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
            rec->mat = temp_rec.mat;
        }
    }

    return hit_anything;
}
