#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"

/* CAMERA DEFINITION */

typedef struct {
    double pixel_samples_scale;
    int samples_per_pixel;
    int max_depth;
    double aspect_ratio;
    int image_height;
    int image_width;
    point3 center;
    point3 pixel00_loc;
    vec3 viewport_u;
    vec3 viewport_v;
    vec3 delta_u;
    vec3 delta_v;
} camera;

void camera_create(camera *cam, double x, double y, double z, int samples_per_pixel, int max_depth, double aspect_ratio, int image_width);
void camera_render(camera *cam, hittable_list *list, FILE *image);
void get_ray(camera *cam, int i, int j, ray *out_ray);
void sample_square(vec3 *out);
void ray_color(ray *r, int depth, hittable_list *list, color *out);

#endif
