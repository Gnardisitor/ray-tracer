#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"

typedef struct {
    double aspect_ratio;
    double focal_length;
    int image_height;
    int image_width;
    point3 center;
    point3 pixel00_loc;
    vec3 viewport_u;
    vec3 viewport_v;
    vec3 delta_u;
    vec3 delta_v;
} camera;

void camera_create(camera *cam, double x, double y, double z, double aspect_ratio, int image_width);
void camera_render(camera *cam, hittable_list *list, FILE *image);
void ray_color(ray *r, hittable_list *list, color *out);

#endif
