#include "main.h"
#include "camera.h"
#include "object.h"
#include "vector.h"

int main(void) {
    /* SCENE SETUP */

    // Create scene
    hittable_list scene;
    scene.count = 0;

    // Create ground sphere
    material ground_material;
    color ground_color = {0.5, 0.5, 0.5};
    create_lambertian(&ground_material, &ground_color);
    add_sphere(&scene, 0.0, -1000.0, 0.0, 1000.0, &ground_material);

    // Create random spheres
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            // Randomly choose material and position
            double choose_material = RAND_DOUBLE;
            point3 center;
            create(&center, a + (0.9 * RAND_DOUBLE), 0.2, b + (0.9 * RAND_DOUBLE));

            
            // Create sphere based on material choice
            if (choose_material < 0.8) {
                // Create lambertian sphere
                color *albedo = malloc(sizeof(color));
                create(albedo, RAND_DOUBLE * RAND_DOUBLE, RAND_DOUBLE * RAND_DOUBLE, RAND_DOUBLE * RAND_DOUBLE);
                material *mat = malloc(sizeof(material));
                create_lambertian(mat, albedo);
                add_sphere(&scene, center[0], center[1], center[2], 0.2, mat);
            } else if (choose_material < 0.95) {
                // Create metal sphere
                color *albedo = malloc(sizeof(color));
                create(albedo, 0.5 * (1 + RAND_DOUBLE), 0.5 * (1 + RAND_DOUBLE), 0.5 * (1 + RAND_DOUBLE));
                double fuzz = 0.5 * RAND_DOUBLE;
                material *mat = malloc(sizeof(material));
                create_metal(mat, albedo, fuzz);
                add_sphere(&scene, center[0], center[1], center[2], 0.2, mat);
            } else {
                // Create dielectric sphere
                material *mat = malloc(sizeof(material));
                create_dielectric(mat, 1.5);
                add_sphere(&scene, center[0], center[1], center[2], 0.2, mat);
            }
        }
    }

    // Add a large dielectric sphere
    material material1;
    create_dielectric(&material1, 1.5);
    add_sphere(&scene, 0.0, 1.0, 0.0, 1.0, &material1);

    // Add a large metal sphere
    material material2;
    color albedo2;
    create(&albedo2, 0.4, 0.2, 0.1);
    create_lambertian(&material2, &albedo2);
    add_sphere(&scene, -4.0, 1.0, 0.0, 1.0, &material2);

    // Add a large metal sphere
    material material3;
    color albedo3;
    create(&albedo3, 0.7, 0.6, 0.5);
    create_metal(&material3, &albedo3, 0.0);
    add_sphere(&scene, 4.0, 1.0, 0.0, 1.0, &material3);

    /* SETUP CAMERA */

    // Set camera position
    double vfov = 20.0;
    vec3 lookfrom, lookat, vup;
    create(&lookfrom, 13.0, 2.0, 3.0);
    create(&lookat, 0.0, 0.0, 0.0);
    create(&vup, 0.0, 1.0, 0.0);

    // Set camera parameters
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 1200;

    // Defocus blur parameters
    double defocus_angle = 0.6;
    double focus_dist = 10.0;

    // Set samples per pixel and max depth
    int samples_per_pixel = 500;
    int max_depth = 50;

    // Create camera
    camera cam;
    camera_create(&cam, &lookfrom, &lookat, &vup, defocus_angle, focus_dist, samples_per_pixel, max_depth, vfov, aspect_ratio, image_width);

    /* RENDER IMAGE */

    // Create image file
    FILE *image = fopen("image.ppm", "wt");
    if (image == NULL) {
        printf("Could not create image\n");
        return EXIT_FAILURE;
    }

    // Render the scene
    camera_render(&cam, &scene, image);
    
    fclose(image);
    return EXIT_SUCCESS;
}
