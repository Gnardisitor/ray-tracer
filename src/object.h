#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

/* HIT RECORD DEFINITION */

struct material; // Forward declaration

typedef struct {
    bool front_face;
    struct material *mat;
    vec3 normal;
    point3 p;
    double t;
} hit_record;

void set_face_normal(ray *r, vec3 *outward_normal, hit_record *rec);

/* INTERVAL DEFINITION */

typedef struct {
    double tmin;
    double tmax;
} interval;

double size(interval *i);
bool contains(interval *i, double x);
bool surrounds(interval *i, double x);
double clamp(interval *i, double x);

/* MATERIAL DEFINITION */

typedef enum {
    LAMBERTIAN,
    DIELECTRIC,
    METAL
} material_type;

typedef struct material {
    material_type type;
    void *data;
    bool (*scatter)(ray *r, hit_record *rec, color *attenuation, ray *scattered);
} material;

/* LAMBERTIAN MATERIAL DEFINITION */

typedef struct {
    color albedo;
} lambertian_data;

void create_lambertian(material *mat, color *albedo);
bool lambertian_scatter(ray *r, hit_record *rec, color *attenuation, ray *scattered);

/* METAL MATERIAL DEFINITION */

typedef struct {
    color albedo;
    double fuzz;
} metal_data;

void create_metal(material *mat, color *albedo, double fuzz);
bool metal_scatter(ray *r, hit_record *rec, color *attenuation, ray *scattered);

/* DIELECTRIC MATERIAL DEFINITION */

typedef struct {
    double refraction_index;
} dielectric_data;

void create_dielectric(material *mat, double refraction_index);
bool dielectric_scatter(ray *r, hit_record *rec, color *attenuation, ray *scattered);
double reflectance(double cosine, double refraction_index);

/* SPHERE DEFINITION */

typedef struct sphere {
    material *mat;
    point3 center;
    double radius;
} sphere;

void sphere_create(sphere *s, point3 *center, double radius, material *mat);
bool sphere_hit(sphere *s, interval *ray_t, ray *r, hit_record *rec);

/* OBJECT DEFINITION */

typedef struct {
    void *object;
    bool (*hit)(void *object, interval *i, ray *r, hit_record *rec);
} hittable;

/* OBJECT LIST DEFINITION */

typedef struct {
    hittable objects[500];
    int count;
} hittable_list;

void add_sphere(hittable_list *list, double x, double y, double z, double radius, material *mat);
bool hit(hittable_list *list, ray *r, interval *ray_t, hit_record *rec);

#endif
