#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"

/* CAMERA DEFINITION */

typedef struct {
    // Sample parameters
    double pixel_samples_scale;
    int samples_per_pixel;
    int max_depth;

    // Viewport parameters
    double aspect_ratio;
    double vfov;
    int image_height;
    int image_width;

    // Defocus blur parameters
    double defocus_angle;
    double focus_dist;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    // Viewport vectors
    point3 center;
    vec3 u, v, w;
    point3 pixel00_loc;
    vec3 viewport_u;
    vec3 viewport_v;
    vec3 delta_u;
    vec3 delta_v;
} camera;

void camera_create(camera *cam, point3 *lookfrom, point3 *lookat, vec3 *vup, double defocus_angle, double focus_dist, int samples_per_pixel, int max_depth, double vfov, double aspect_ratio, int image_width);
void camera_render(camera *cam, hittable_list *list, FILE *image);
void get_ray(camera *cam, int i, int j, ray *out_ray);
void sample_square(vec3 *out);
void defocus_disk_sample(camera *cam, point3 *out);
void ray_color(ray *r, int depth, hittable_list *list, color *out);

#endif
