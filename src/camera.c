#include "camera.h"
#include "main.h"

void camera_create(camera *cam, double x, double y, double z, int samples_per_pixel, double aspect_ratio, int image_width) {
    // Initialize camera parameters
    cam->aspect_ratio = aspect_ratio;
    cam->image_width = image_width;
    cam->image_height = (int)(image_width / aspect_ratio);
    if (cam->image_height < 1) cam->image_height = 1;
    create(&cam->center, x, y, z);

    // Set samples per pixel and pixel samples scale
    cam->samples_per_pixel = samples_per_pixel;
    cam->pixel_samples_scale = 1.0 / (double)samples_per_pixel;

    // Calculate viewport dimensions
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * aspect_ratio;

    // Calculate vectors across viewport
    create(&cam->viewport_u, viewport_width, 0.0, 0.0);
    create(&cam->viewport_v, 0.0, -viewport_height, 0.0);

    // Calculate delta vectors from pixel to pixel
    divide(&cam->viewport_u, (double)image_width, &cam->delta_u);
    divide(&cam->viewport_v, (double)cam->image_height, &cam->delta_v);

    // Calculate location of upper left pixel
    vec3 viewport_upper_left;
    viewport_upper_left[0] = cam->center[0] - (cam->viewport_u[0] / 2) - (cam->viewport_v[0] / 2);
    viewport_upper_left[1] = cam->center[1] - (cam->viewport_u[1] / 2) - (cam->viewport_v[1] / 2);
    viewport_upper_left[2] = cam->center[2] - focal_length - (cam->viewport_u[2] / 2) - (cam->viewport_v[2] / 2);

    // Calculate pixel00 location
    cam->pixel00_loc[0] = viewport_upper_left[0] + 0.5 * (cam->delta_u[0] + cam->delta_v[0]);
    cam->pixel00_loc[1] = viewport_upper_left[1] + 0.5 * (cam->delta_u[1] + cam->delta_v[1]);
    cam->pixel00_loc[2] = viewport_upper_left[2] + 0.5 * (cam->delta_u[2] + cam->delta_v[2]);
}

void camera_render(camera *cam, hittable_list *list, FILE *image) {
    // Write PPM header
    fprintf(image, "P3\n%d %d\n255\n", cam->image_width, cam->image_height);

    // Create variables for pixel color and ray
    //point3 pixel_center;
    //vec3 ray_direction;
    color pixel_color;
    ray r;

    // Render the image
    for (int j = 0; j < cam->image_height; j++) {
        for (int i = 0; i < cam->image_width; i++) {
            /*
            // Calculate pixel center location
            pixel_center[0] = cam->pixel00_loc[0] + (i * cam->delta_u[0]) + (j * cam->delta_v[0]);
            pixel_center[1] = cam->pixel00_loc[1] + (i * cam->delta_u[1]) + (j * cam->delta_v[1]);
            pixel_center[2] = cam->pixel00_loc[2] + (i * cam->delta_u[2]) + (j * cam->delta_v[2]);

            // Calculate ray direction and create ray
            subtract(&pixel_center, &cam->center, &ray_direction);
            ray_create(&r, &cam->center, &ray_direction);

            // Calculate color for the ray and write to image
            ray_color(&r, list, &pixel_color);
            write_color(image, &pixel_color);
            */
            create(&pixel_color, 0.0, 0.0, 0.0);
            for (int s = 0; s < cam->samples_per_pixel; s++) {
                get_ray(cam, i, j, &r);
                ray_color(&r, list, &pixel_color);
            }
            multiply(&pixel_color, cam->pixel_samples_scale, &pixel_color);
            write_color(image, &pixel_color);
        }
    }

    printf("Image finished rendering\n");
}

void sample_square(vec3 *out) {
    (*out)[0] = RAND_DOUBLE - 0.5;
    (*out)[1] = RAND_DOUBLE - 0.5;
    (*out)[2] = 0.0;
}

void get_ray(camera *cam, int i, int j, ray *out_ray) {
    // Get point to use for ray end
    vec3 offset, pixel_sample;
    sample_square(&offset);
    pixel_sample[0] = cam->pixel00_loc[0] + ((i + offset[0]) * cam->delta_u[0]) + ((j + offset[1]) * cam->delta_v[0]);
    pixel_sample[1] = cam->pixel00_loc[1] + ((i + offset[0]) * cam->delta_u[1]) + ((j + offset[1]) * cam->delta_v[1]);
    pixel_sample[2] = cam->pixel00_loc[2] + ((i + offset[0]) * cam->delta_u[2]) + ((j + offset[1]) * cam->delta_v[2]);

    // Set ray from camera center to pixel sample
    (*out_ray).origin[0] = cam->center[0];
    (*out_ray).origin[1] = cam->center[1];
    (*out_ray).origin[2] = cam->center[2];
    (*out_ray).direction[0] = pixel_sample[0] - cam->center[0];
    (*out_ray).direction[1] = pixel_sample[1] - cam->center[1];
    (*out_ray).direction[2] = pixel_sample[2] - cam->center[2];
}

void ray_color(ray *r, hittable_list *list, color *out) {
    // Initialize hit record with no hit
    hit_record rec;
    rec.t = -1.0;
    rec.p[0] = 0.0;
    rec.p[1] = 0.0;
    rec.p[2] = 0.0;
    rec.normal[0] = 0.0;
    rec.normal[1] = 0.0;
    rec.normal[2] = 0.0;

    // Return color based on normal if ray hits an object
    interval ray_t = {0.0, INFINITY};
    if (hit(list, r, &ray_t, &rec)) {
        (*out)[0] += 0.5 * (rec.normal[0] + 1.0);
        (*out)[1] += 0.5 * (rec.normal[1] + 1.0);
        (*out)[2] += 0.5 * (rec.normal[2] + 1.0);
        return;
    }

    // Compute gradient on Y axis for background
    vec3 unit_direction;
    unit_vector(&r->direction, &unit_direction);
    double a = 0.5 * (unit_direction[1] + 1.0);
    (*out)[0] += (1.0 - a) + a * 0.5;
    (*out)[1] += (1.0 - a) + a * 0.7;
    (*out)[2] += (1.0 - a) + a * 1.0;
}
