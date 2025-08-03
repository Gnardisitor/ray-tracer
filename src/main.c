#include "main.h"
#include "camera.h"
#include "object.h"
#include "vector.h"

/* SCENE CREATION */

int main(void) {
    // Create albedo colors for materials
    color ground_color = {0.8, 0.8, 0.0};
    color center_color = {0.1, 0.2, 0.5};
    color right_color = {0.8, 0.6, 0.2};

    // Create materials
    material material_ground, material_center, material_left, material_bubble, material_right;
    create_lambertian(&material_ground, &ground_color);
    create_lambertian(&material_center, &center_color);
    create_dielectric(&material_left, 1.5);
    create_dielectric(&material_bubble, 1.0 / 1.5);
    create_metal(&material_right, &right_color, 1.0);

    // Create world
    hittable_list world;
    world.count = 0;
    add_sphere(&world, 0.0, -100.5, -1.0, 100.0, &material_ground);
    add_sphere(&world, 0.0, 0.0, -1.2, 0.5, &material_center);
    add_sphere(&world, -1.0, 0.0, -1.0, 0.5, &material_left);
    add_sphere(&world, -1.0, 0.0, -1.0, 0.4, &material_bubble);
    add_sphere(&world, 1.0, 0.0, -1.0, 0.5, &material_right);

    /*
    double R = cos(PI / 4.0);
    material material_left, material_right;
    color left_color = {0.0, 0.0, 1.0};
    color right_color = {1.0, 0.0, 0.0};
    create_lambertian(&material_left, &left_color);
    create_lambertian(&material_right, &right_color);

    hittable_list world;
    world.count = 0;
    add_sphere(&world, -R, 0.0, -1.0, R, &material_left);
    add_sphere(&world, R, 0.0, -1.0, R, &material_right);
    */

    // Create camera
    camera cam;
    double vfov = 75.0;
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 100;
    int max_depth = 10;
    camera_create(&cam, 0.0, 0.0, 0.0, samples_per_pixel, max_depth, vfov, aspect_ratio, image_width);

    // Create image file
    FILE *image = fopen("image.ppm", "wt");
    if (image == NULL) {
        printf("Could not create image\n");
        return EXIT_FAILURE;
    }

    // Render the scene
    camera_render(&cam, &world, image);
    
    fclose(image);
    return EXIT_SUCCESS;
}
