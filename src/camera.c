#include "camera.h"

/* CAMERA DEFINITION */

void camera_create(camera *cam, point3 *lookfrom, point3 *lookat, vec3 *vup, int samples_per_pixel, int max_depth, double vfov, double aspect_ratio, int image_width) {
    // Initialize camera parameters
    cam->aspect_ratio = aspect_ratio;
    cam->image_width = image_width;
    cam->image_height = (int)(image_width / aspect_ratio);
    if (cam->image_height < 1) cam->image_height = 1;
    create(&cam->center, (*lookfrom)[0], (*lookfrom)[1], (*lookfrom)[2]);

    // Set samples per pixel and pixel samples scale
    cam->samples_per_pixel = samples_per_pixel;
    cam->pixel_samples_scale = 1.0 / (double)samples_per_pixel;
    cam->max_depth = max_depth;

    // Calculate viewport dimensions
    vec3 vector;
    subtract(lookat, lookfrom, &vector);
    double focal_length = length(&vector);
    double theta = DEG_TO_RAD(vfov); 
    double h = tan(theta / 2.0);
    double viewport_height = 2.0 * h * focal_length;
    double viewport_width = viewport_height * aspect_ratio;

    // Calculate u,v, w unit vectors from camera coordinates
    vec3 temp_u, temp_w;
    subtract(lookfrom, lookat, &temp_w);
    unit_vector(&temp_w, &cam->w);
    cross(vup, &cam->w, &temp_u);
    unit_vector(&temp_u, &cam->u);
    cross(&cam->w, &cam->u, &cam->v);

    // Calculate vectors across viewport
    vec3 nv;
    negate(&cam->v, &nv);
    multiply(&cam->u, viewport_width, &cam->viewport_u);
    multiply(&nv, viewport_height, &cam->viewport_v);

    // Calculate delta vectors from pixel to pixel
    divide(&cam->viewport_u, (double)image_width, &cam->delta_u);
    divide(&cam->viewport_v, (double)cam->image_height, &cam->delta_v);

    // Calculate location of upper left pixel
    vec3 viewport_upper_left;
    viewport_upper_left[0] = cam->center[0] - (focal_length * cam->w[0]) - (cam->viewport_u[0] / 2) - (cam->viewport_v[0] / 2);
    viewport_upper_left[1] = cam->center[1] - (focal_length * cam->w[1]) - (cam->viewport_u[1] / 2) - (cam->viewport_v[1] / 2);
    viewport_upper_left[2] = cam->center[2] - (focal_length * cam->w[2]) - (cam->viewport_u[2] / 2) - (cam->viewport_v[2] / 2);

    // Calculate pixel00 location
    cam->pixel00_loc[0] = viewport_upper_left[0] + 0.5 * (cam->delta_u[0] + cam->delta_v[0]);
    cam->pixel00_loc[1] = viewport_upper_left[1] + 0.5 * (cam->delta_u[1] + cam->delta_v[1]);
    cam->pixel00_loc[2] = viewport_upper_left[2] + 0.5 * (cam->delta_u[2] + cam->delta_v[2]);
}

void camera_render(camera *cam, hittable_list *list, FILE *image) {
    // Write PPM header
    fprintf(image, "P3\n%d %d\n255\n", cam->image_width, cam->image_height);

    // Create variables for pixel color and ray
    color pixel_color, sample;
    ray r;

    // Render the image
    for (int j = 0; j < cam->image_height; j++) {
        // Show progress
        double percent = 100.0 * (double)(j + 1) / (double)cam->image_height;
        printf("\rRendering %.1f%%", percent);
        fflush(stdout);

        for (int i = 0; i < cam->image_width; i++) {
            // Set pixel color to black to begin
            create(&pixel_color, 0.0, 0.0, 0.0);

            // Accumulate color for each sample
            for (int s = 0; s < cam->samples_per_pixel; s++) {
                get_ray(cam, i, j, &r);
                ray_color(&r, cam->max_depth, list, &sample);
                add(&pixel_color, &sample, &pixel_color);
            }

            // Scale pixel color by samples per pixel and write to image
            multiply(&pixel_color, cam->pixel_samples_scale, &pixel_color);
            write_color(image, &pixel_color);
        }
    }

    printf("\nImage finished rendering\n");
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

void ray_color(ray *r, int depth, hittable_list *list, color *out) {
    // Check for maximum recursion depth
    if (depth <= 0) {
        (*out)[0] = 0.0;
        (*out)[1] = 0.0;
        (*out)[2] = 0.0;
        return;
    }

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
    interval ray_t = {0.001, INFINITY};
    if (hit(list, r, &ray_t, &rec)) {
        /*
        // Find bounce direction on hemisphere
        vec3 random, direction;
        random_on_hemisphere(&random, &rec.normal);
        add(&rec.p, &random, &direction);

        // Create bounce ray from hit point
        ray bounce_ray;
        create(&bounce_ray.origin, rec.p[0], rec.p[1], rec.p[2]);
        create(&bounce_ray.direction, direction[0], direction[1], direction[2]);

        // Recursively get color from bounce ray
        color bounce_color;
        ray_color(&bounce_ray, depth - 1, list, &bounce_color);

        // Scale bounce color by normal
        double reflectance = 0.3;
        multiply(&bounce_color, reflectance, out);
        return;
        */
        ray scattered;
        color attenuation;
        if (rec.mat->scatter(r, &rec, &attenuation, &scattered)) {
            if (rec.mat == NULL) {
            fprintf(stderr, "ERROR: rec.mat is NULL at t=%f\n", rec.t);
            exit(EXIT_FAILURE);
            }
            if (rec.mat->scatter == NULL) {
                fprintf(stderr, "ERROR: rec.mat->scatter is NULL at t=%f\n", rec.t);
                exit(EXIT_FAILURE);
            }

            // Recursively get color from scattered ray
            color scattered_color;
            ray_color(&scattered, depth - 1, list, &scattered_color);

            // Scale scattered color by attenuation
            (*out)[0] = attenuation[0] * scattered_color[0];
            (*out)[1] = attenuation[1] * scattered_color[1];
            (*out)[2] = attenuation[2] * scattered_color[2];
            return;
        }

        // If material does not scatter, return background color
        create(out, 0.0, 0.0, 0.0);
        return;
    }

    // Compute gradient on Y axis for background
    vec3 unit_direction;
    unit_vector(&r->direction, &unit_direction);
    double a = 0.5 * (unit_direction[1] + 1.0);
    (*out)[0] = (1.0 - a) + a * 0.5;
    (*out)[1] = (1.0 - a) + a * 0.7;
    (*out)[2] = (1.0 - a) + a * 1.0;
}
