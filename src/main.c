#include "main.h"

hittable_list world;

void ray_color(ray *r, color *out) {
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
    if (hit(&world, r, 0.0, INFINITY, &rec)) {
        (*out)[0] = 0.5 * (rec.normal[0] + 1.0);
        (*out)[1] = 0.5 * (rec.normal[1] + 1.0);
        (*out)[2] = 0.5 * (rec.normal[2] + 1.0);
        return;
    }

    // Compute gradient on Y axis
    vec3 unit_direction;
    unit_vector(&r->direction, &unit_direction);
    double a = 0.5 * (unit_direction[1] + 1.0);
    (*out)[0] = (1.0 - a) + a * 0.5;
    (*out)[1] = (1.0 - a) + a * 0.7;
    (*out)[2] = (1.0 - a) + a * 1.0;
}

int main(void) {
    // Compute aspect ratio and image dimensions
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = (int)(image_width / aspect_ratio);
    if (image_height < 1) image_height = 1;

    // Create world
    world.count = 0;
    add_sphere(&world, 0.0, 0.0, -1.0, 0.5);
    add_sphere(&world, 0.0, -100.5, -1.0, 100.0);

    // Camera parameters
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)image_width / (double)image_height);
    point3 camera_center;
    create(&camera_center, 0.0, 0.0, 0.0);

    // Calculate vectors across viewport
    vec3 viewport_u, viewport_v;
    create(&viewport_u, viewport_width, 0.0, 0.0);
    create(&viewport_v, 0.0, -viewport_height, 0.0);

    // Calculate delta vectors from pixel to pixel
    vec3 delta_u, delta_v;
    divide(&viewport_u, (double)image_width, &delta_u);
    divide(&viewport_v, (double)image_height, &delta_v);

    // Calcualte location of upper left pixel
    point3 viewport_upper_left, pixel00_loc;
    // vec3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    viewport_upper_left[0] = camera_center[0] - (viewport_u[0] / 2) - (viewport_v[0] / 2);
    viewport_upper_left[1] = camera_center[1] - (viewport_u[1] / 2) - (viewport_v[1] / 2);
    viewport_upper_left[2] = camera_center[2] - focal_length - (viewport_u[2] / 2) - (viewport_v[2] / 2);
    // vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    pixel00_loc[0] = viewport_upper_left[0] + 0.5 * (delta_u[0] + delta_v[0]);
    pixel00_loc[1] = viewport_upper_left[1] + 0.5 * (delta_u[1] + delta_v[1]);
    pixel00_loc[2] = viewport_upper_left[2] + 0.5 * (delta_u[2] + delta_v[2]);

    // Create image file
    FILE *image = fopen("image.ppm", "wt");
    if (image == NULL) {
        printf("Could not create image\n");
        return EXIT_FAILURE;
    }

    // Write PPM header
    fprintf(image, "P3\n%d %d\n255\n", image_width, image_height);

    // Create variables for pixel color and ray
    point3 pixel_center;
    vec3 ray_direction;
    color pixel_color;
    ray r;

    // Render the image
    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            // Calculate pixel center location
            pixel_center[0] = pixel00_loc[0] + (i * delta_u[0]) + (j * delta_v[0]);
            pixel_center[1] = pixel00_loc[1] + (i * delta_u[1]) + (j * delta_v[1]);
            pixel_center[2] = pixel00_loc[2] + (i * delta_u[2]) + (j * delta_v[2]);

            // Calculate ray direction and create ray
            subtract(&pixel_center, &camera_center, &ray_direction);
            ray_create(&r, &camera_center, &ray_direction);

            // Calculate color for the ray and write to image
            ray_color(&r, &pixel_color);
            write_color(image, &pixel_color);
        }
    }

    // Close image file
    fclose(image);
    printf("Image finished rendering\n");
    return EXIT_SUCCESS;
}
